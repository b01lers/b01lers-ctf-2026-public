import { useEffect, useState } from 'react';
import { useNavigate, useSearchParams } from 'react-router';
import { z } from 'zod/v4';
import StyledNotebook from './StyledNotebook';
import type { Notebook } from './lib/types';


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
