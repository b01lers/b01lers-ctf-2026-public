export type Notebook = {
    metadata: Record<string, any>,
    nbformat: number,
    nbformat_minor: number,
    cells: NotebookCell[]
}

// type NotebookMetadata = {
//     kernel_info?: {
//         name: string
//     },
//     language_info?: {
//         name: string,
//         version?: string,
//         file_extension?: string,
//         codemirror_mode?: {
//             name: string,
//             version: string | number,
//         },
//         mimetype?: string,
//         nbconvert_exporter?: string,
//         pygments_lexer?: string
//     }
// }

export type NotebookCell = MarkdownCell | CodeCell | RawCell;

interface BaseCell {
    id?: string,
    source: string | string[],
    metadata: Record<string, any>
}

export interface MarkdownCell extends BaseCell {
    cell_type: 'markdown',
    attachments?: Record<string, Record<string, string>>
}

export interface CodeCell extends BaseCell {
    cell_type: 'code',
    execution_count: number | null,
    outputs: CodeCellOutput[]
}

export type CodeCellOutput = CodeCellStreamOutput
    | CodeCellDisplayDataOutput
    | CodeCellExecuteResultOutput
    | CodeCellErrorOutput;

type CodeCellStreamOutput = {
    output_type: 'stream',
    name: 'stdout' | 'stderr',
    text: string,
}

type CodeCellDisplayDataOutput = {
    output_type: 'display_data',

    /**
     * A {@Link https://nbformat.readthedocs.io/en/latest/format_description.html#display-data "mime bundle"}
     * associating mime types to their corresponding data.
     */
    data: Record<string, any>

    /**
     * A {@Link https://nbformat.readthedocs.io/en/latest/format_description.html#display-data "mime bundle"}
     * associating mime types to their corresponding metadata.
     */
    metadata: Record<string, any>
}

type CodeCellExecuteResultOutput = {
    output_type: 'execute_result',
    execution_count: number,

    /**
     * A {@Link https://nbformat.readthedocs.io/en/latest/format_description.html#display-data "mime bundle"}
     * associating mime types to their corresponding data.
     */
    data: Record<string, any>

    /**
     * A {@Link https://nbformat.readthedocs.io/en/latest/format_description.html#display-data "mime bundle"}
     * associating mime types to their corresponding metadata.
     */
    metadata: Record<string, any>
}

type CodeCellErrorOutput = {
    output_type: 'error',
    ename: string,
    evalue: string,
    traceback: string[]
}

interface RawCell extends BaseCell {
    cell_type: 'raw'
}
