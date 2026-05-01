import { loadPyodide } from "https://cdn.jsdelivr.net/pyodide/v0.29.0/full/pyodide.mjs";


const pipliteWheelUrl = 'https://jupyter.org/try-jupyter/extensions/@jupyterlite/pyodide-kernel-extension/static/pypi/piplite-0.7.0-py3-none-any.whl'
const pipliteUrls = [
    'https://jupyter.org/try-jupyter/extensions/@jupyterlite/pyodide-kernel-extension/static/pypi/all.json',
    // 'https://jupyter.org/try-jupyter/extensions/@jupyterlite/pyodide-kernel-extension/static/pypi/ipykernel-6.9.2-py3-none-any.whl',
    // 'https://jupyter.org/try-jupyter/extensions/@jupyterlite/pyodide-kernel-extension/static/pypi/pyodide_kernel-0.6.0-py3-none-any.whl',
    // 'https://jupyter.org/try-jupyter/extensions/@jupyterlite/pyodide-kernel-extension/static/pypi/widgetsnbextension-3.6.999-py3-none-any.whl',
    // 'https://jupyter.org/try-jupyter/extensions/@jupyterlite/pyodide-kernel-extension/static/pypi/widgetsnbextension-4.0.999-py3-none-any.whl'
]


// Simple JS lock, based on https://jackpordi.com/posts/locks-in-js-because-why-not
class Lock {
    acquired = false;
    queue = [];

    async acquire() {
        if (!this.acquired) {
            this.acquired = true;
        } else {
            return new Promise((resolve, _) => {
                this.queue.push(resolve);
            });
        }
    }

    async release() {
        if (this.queue.length === 0 && this.acquired) {
            this.acquired = false;
            return;
        }

        const continuation = this.queue.shift();
        return new Promise((res) => {
            continuation();
            res();
        });
    }
}


class PyodideWorker {
    constructor() {
        this.initialized = this.initialize();
        this.executeLock = new Lock();
    }

    async initialize() {
        this.pyodide = await loadPyodide();

        await this.pyodide.loadPackage(['micropip']);
        await this.pyodide.runPythonAsync(`
            import micropip
            await micropip.install('${pipliteWheelUrl}', keep_going=True)
        `);
        await this.pyodide.runPythonAsync(`
            import piplite.piplite
            piplite.piplite._PIPLITE_DISABLE_PYPI = False
            piplite.piplite._PIPLITE_URLS = ${JSON.stringify(pipliteUrls)}
        `);

        const scriptLines = [];
        for (const pkg of ["sqlite3", "ipykernel", "comm", "pyodide_kernel", "jedi", "ipython"]) {
            scriptLines.push(`await piplite.install('${pkg}', keep_going=True)`);
        }

        scriptLines.push('import pyodide_kernel');
        await this.pyodide.runPythonAsync(scriptLines.join('\n'));

        const { globals } = this.pyodide;
        this.kernel = globals.get('pyodide_kernel').kernel_instance.copy();
        this.stdout_stream = globals.get('pyodide_kernel').stdout_stream.copy();
        this.stderr_stream = globals.get('pyodide_kernel').stderr_stream.copy();
        this.interpreter = this.kernel.interpreter.copy();
        // this.interpreter.send_comm = this.sendComm.bind(this);

        self.postMessage({
            output_type: 'ready'
        });
    }

    mapToObject(obj) {
        const out = obj instanceof Array ? [] : {};
        const entries = obj instanceof Map ? obj.entries() : Object.entries(obj);

        for (const [key, value] of entries) {
            out[key] = value instanceof Map || value instanceof Array
                ? this.mapToObject(value)
                : value;
        }
        return out;
    }

    formatResult(res) {
        if (!(res instanceof this.pyodide.ffi.PyProxy)) {
            return res;
        }
        const m = res.toJs();
        return this.mapToObject(m);
    }

    async execute(id, content) {
        await this.executeLock.acquire();
        await this.pyodide.loadPackagesFromImports(content);

        // Patch out `execute_result` for safety :D
        const publishExecutionResult = (prompt_count, data, metadata) => {
            self.postMessage({
                id,
                output_type: 'execute_result',
                execution_count: prompt_count,
                data: { 'text/plain': '<\'execute_result\' output disabled due to security reasons>' },
                metadata: this.formatResult(metadata),
            });
        };

        const publishExecutionError = (ename, evalue, traceback) => {
            self.postMessage({
                id,
                output_type: 'error',
                ename: ename,
                evalue: evalue,
                traceback: traceback,
            });
        };

        const clearOutputCallback = (wait) => {
            self.postMessage({
                id,
                output_type: 'clear_output',
                wait: this.formatResult(wait),
            });
        };

        // Patch out `display_data` for security :D
        const displayDataCallback = (data, metadata, transient) => {
            self.postMessage({
                id,
                output_type: 'display_data',
                data: { 'text/plain': '<\'display_data\' output disabled due to security reasons>' },
                metadata: this.formatResult(metadata),
                transient: this.formatResult(transient),
            });
        };

        const updateDisplayDataCallback = (data, metadata, transient) => {
            self.postMessage({
                id,
                output_type: 'update_display_data',
                data: this.formatResult(data),
                metadata: this.formatResult(metadata),
                transient: this.formatResult(transient),
            });
        };

        const publishStreamCallback = (name, text) => {
            self.postMessage({
                id,
                output_type: 'stream',
                name: this.formatResult(name),
                text: this.formatResult(text),
            });
        };

        this.stdout_stream.publish_stream_callback = publishStreamCallback;
        this.stderr_stream.publish_stream_callback = publishStreamCallback;
        this.interpreter.display_pub.clear_output_callback = clearOutputCallback;
        this.interpreter.display_pub.display_data_callback = displayDataCallback;
        this.interpreter.display_pub.update_display_data_callback = updateDisplayDataCallback;
        this.interpreter.displayhook.publish_execution_result = publishExecutionResult;
        // this.interpreter.input = this.input.bind(this);
        // this.interpreter.getpass = this.getpass.bind(this);

        const res = await this.kernel.run(content);
        const results = this.formatResult(res);

        if (results['status'] === 'error') {
            publishExecutionError(results['ename'], results['evalue'], results['traceback']);
        }

        self.postMessage({
            id,
            output_type: 'done'
        })

        await this.executeLock.release();
        return results;
    }
}


const worker = new PyodideWorker();

self.onmessage = async (event) => {
    // Ensure initialization is complete
    await worker.initialized;

    const { id, python } = event.data;
    await worker.execute(id, python);
};
