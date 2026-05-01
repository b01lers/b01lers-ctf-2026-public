import type { MarkdownCell } from './types';
import type { Pluggable } from 'unified';
import Markdown from 'react-markdown';


type JupyterNotebookMarkdownCellProps = {
    cell: MarkdownCell,
    markdownClassName?: string,
    remarkPlugins?: Pluggable[],
    rehypePlugins?: Pluggable[]
}

export default function JupyterNotebookMarkdownCell(props: JupyterNotebookMarkdownCellProps) {
    const source = Array.isArray(props.cell.source) ? props.cell.source.join('') : props.cell.source;

    return (
        <div
            style={{ paddingLeft: '4rem' }}
            className={props.markdownClassName}
        >
            <Markdown
                remarkPlugins={props.remarkPlugins}
                rehypePlugins={props.rehypePlugins}
            >
                {source}
            </Markdown>
        </div>
    )
}
