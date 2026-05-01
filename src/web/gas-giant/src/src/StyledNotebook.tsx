import JupyterNotebook from './lib/JupyterNotebook';
import type { Notebook } from './lib/types';
import remarkMath from 'remark-math';
import rehypeKatex from 'rehype-katex';


export default function StyledNotebook(props: { notebook: Notebook, className: string }) {
    return (
        <JupyterNotebook
            notebook={props.notebook}
            remarkPlugins={[remarkMath]}
            rehypePlugins={[rehypeKatex]}
            wrapperClassName={props.className}
            markdownClassName="markdown mb-4"
            codeCellClassName="group my-2"
            executionCountClassName="font-mono group-data-active:text-sky-600 group-data-edited:text-orange-400!"
            runButtonClassName="cursor-pointer text-right px-1"
            codeEditorClassName="border border-black/10 bg-[rgb(245_245_245)] [&>textarea]:focus:outline-sky-600"
            streamOutputClassName="mt-2 px-2 py-1 overflow-x-auto"
            errorOutputClassName="mt-2 px-2 py-1 overflow-x-auto bg-red-500/20"
            indicatorClassName="hidden group-data-active:block absolute left-1 w-2 rounded-xs h-full bg-sky-600 group-data-edited:bg-orange-400"
        />
    )
}
