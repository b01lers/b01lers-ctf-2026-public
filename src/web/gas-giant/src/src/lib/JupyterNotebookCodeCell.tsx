import { memo, useState } from 'react';
import Editor from 'react-simple-code-editor';
import { highlight, languages } from 'prismjs';
import 'prismjs/components/prism-python';

// Components
import Ansi from './Ansi';

// Utils
import type { CodeCell, CodeCellOutput as CodeCellOutputData } from './types';


type JupyterNotebookCodeCellProps = {
    cell: CodeCell,
    index: number,
    trusted?: boolean,

    ready: boolean,
    executePython: (code: string, callback: (message: CodeCellOutputData) => void) => void,
    focusedIndex: number,
    setFocusedIndex: (index: number) => void,
    currentCount: number,
    setCurrentCount: (c: number) => void,

    codeCellClassName?: string,
    executionCountClassName?: string,
    runButtonClassName?: string,
    codeEditorClassName?: string,
    streamOutputClassName?: string,
    errorOutputClassName?: string,
    indicatorClassName?: string,
}

export default function JupyterNotebookCodeCell(props: JupyterNotebookCodeCellProps) {
    const [code, setCode] = useState(Array.isArray(props.cell.source) ? props.cell.source.join('') : props.cell.source);
    const [prevCode, setPrevCode] = useState(code);

    const [outputs, setOutputs] = useState(props.cell.outputs);
    const [count, setCount] = useState(props.cell.execution_count);

    // Run the code in this cell by updating the execution count and sending the code
    // content to the Pyodide worker.
    function execute() {
        setPrevCode(code);
        props.setFocusedIndex(props.index);

        setCount(props.currentCount);
        props.setCurrentCount(props.currentCount + 1);

        setOutputs([]);

        let currStream: string | null = null;
        let currText: string | null = null;

        props.executePython(code, (e) => {
            // For stream outputs, we need to merge new entries with previous ones if they come consecutively.
            if (e.output_type === 'stream' && currStream === e.name) {
                currText += e.text;
                e.text = currText!;

                setOutputs((o) => [...o.slice(0, -1), e]);
            } else {
                if (e.output_type === 'stream') {
                    currStream = e.name;
                    currText = e.text;
                }
                setOutputs((o) => [...o, e]);
            }
        });
    }

    const edited = code !== prevCode;

    return (
        <div
            style={{ display: 'flex', gap: '0.5rem', position: 'relative', paddingLeft: '4rem' }}
            className={props.codeCellClassName}
            data-active={props.focusedIndex === props.index || undefined}
            data-edited={edited || undefined}
        >
            {props.indicatorClassName && (
                <div className={props.indicatorClassName} />
            )}

            <div
                style={{
                    display: 'flex',
                    flexDirection: 'column',
                    position: 'absolute',
                    left: 0,
                    width: '3.5rem',
                    opacity: props.ready ? 1 : 0.5
                }}
            >
                <p
                    style={{ textAlign: 'right' }}
                    className={props.executionCountClassName}
                >
                    {edited && '•'}[{count ?? ' '}]:
                </p>
                <button
                    className={props.runButtonClassName}
                    disabled={!props.ready}
                    onClick={execute}
                >
                    ▶
                </button>
            </div>
            <div style={{ width: '100%' }}>
                <Editor
                    value={code}
                    onValueChange={(code) => setCode(code)}
                    highlight={(code) => highlight(code, languages.python, 'python')}
                    padding={{
                        top: '0.5rem',
                        bottom: '0.5rem',
                        left: '1rem',
                        right: '1rem',
                    }}
                    style={{ fontFamily: '"Fira code", "Fira Mono", monospace' }}
                    className={props.codeEditorClassName}
                    onFocus={() => props.setFocusedIndex(props.index)}
                >
                    {code}
                </Editor>

                {outputs.map((output, i) => (
                    <MemoizedCodeCellOutput
                        trusted={props.trusted}
                        output={output}
                        streamOutputClassName={props.streamOutputClassName}
                        errorOutputClassName={props.errorOutputClassName}
                        key={JSON.stringify(output) + i}
                    />
                ))}
            </div>
        </div>
    )
}

type CodeCellOutputProps = {
    trusted?: boolean,
    output: CodeCellOutputData,
    streamOutputClassName?: string,
    errorOutputClassName?: string,
}

function CodeCellOutput(props: CodeCellOutputProps) {
    if (props.output.output_type === 'stream') return props.output.name === 'stdout' ? (
        <pre className={props.streamOutputClassName}>
            {props.output.text}
        </pre>
    ) : (
        <pre className={props.errorOutputClassName}>
            {props.output.text}
        </pre>
    )

    if (props.output.output_type === 'error') {
        const output = props.output.traceback.join('\n');
        return (
            <Ansi className={props.errorOutputClassName}>
                {output}
            </Ansi>
        )
    }

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

        // If nothing else matches, fall back to `text/plain` output.
        if (mimes['text/plain']) return (
            <pre className={props.streamOutputClassName}>
                {mimes['text/plain']}
            </pre>
        )

        return null;
    }

    return null;
}

const MemoizedCodeCellOutput = memo(CodeCellOutput);
