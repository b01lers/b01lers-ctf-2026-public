'use client'

import { useCallback, useEffect, useState } from 'react';
import PyodideWorker from './pyodideWorker.mjs?worker&inline';
import type { Pluggable } from 'unified';

// Components
import JupyterNotebookCodeCell from './JupyterNotebookCodeCell';
import JupyterNotebookMarkdownCell from './JupyterNotebookMarkdownCell';

// Utils
import type { CodeCellOutput, Notebook } from './types';
import { getId, receiveType } from './pyodideWorkerClient';


type JupyterNotebookProps = {
    notebook: Notebook,
    trusted?: boolean,

    remarkPlugins?: Pluggable[],
    rehypePlugins?: Pluggable[]

    wrapperClassName?: string,
    markdownClassName?: string,
    codeCellClassName?: string,
    executionCountClassName?: string,
    runButtonClassName?: string,
    codeEditorClassName?: string,
    streamOutputClassName?: string,
    errorOutputClassName?: string,
    indicatorClassName?: string,
}

export default function JupyterNotebook(props: JupyterNotebookProps) {
    const [worker, setWorker] = useState<Worker | null>(null);
    const [ready, setReady] = useState(false);

    const [focusedIndex, setFocusedIndex] = useState(-1);
    const [currentCount, setCurrentCount] = useState(1);

    useEffect(() => {
        async function loadPyodide() {
            const worker = new PyodideWorker();
            setWorker(worker);

            await receiveType(worker, 'ready');
            setReady(true);
        }

        void loadPyodide();
    }, []);

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

    return (
        <div
            style={{ position: 'relative' }}
            className={props.wrapperClassName}
        >
            {props.notebook.cells.map((cell, i) => {
                if (cell.cell_type === 'code') return (
                    <JupyterNotebookCodeCell
                        key={i}
                        index={i}
                        cell={cell}
                        trusted={props.trusted}
                        ready={ready}
                        executePython={executePython}
                        focusedIndex={focusedIndex}
                        setFocusedIndex={setFocusedIndex}
                        currentCount={currentCount}
                        setCurrentCount={setCurrentCount}

                        codeCellClassName={props.codeCellClassName}
                        executionCountClassName={props.executionCountClassName}
                        runButtonClassName={props.runButtonClassName}
                        codeEditorClassName={props.codeEditorClassName}
                        streamOutputClassName={props.streamOutputClassName}
                        errorOutputClassName={props.errorOutputClassName}
                        indicatorClassName={props.indicatorClassName}
                    />
                );

                if (cell.cell_type === 'markdown') return (
                    <JupyterNotebookMarkdownCell
                        cell={cell}
                        remarkPlugins={props.remarkPlugins}
                        rehypePlugins={props.rehypePlugins}
                        markdownClassName={props.markdownClassName}
                        key={i}
                    />
                );

                return null;
            })}
        </div>
    )
}
