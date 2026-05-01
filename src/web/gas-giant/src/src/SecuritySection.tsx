import { useState } from 'react';


export default function SecuritySection() {
    const [url, setUrl] = useState('');
    const [message, setMessage] = useState('');
    const [error, setError] = useState('');

    return (
        <>
            <h2 className="text-2xl font-bold mt-10 mb-2">
                Security
            </h2>
            <p className="text-pretty">
                The <strong>b01lerLite</strong> team takes security violations very seriously. If you are aware of any
                URL hosting insecure or dangerous content, please report it to the admin using the form below:
            </p>
            <form
                className="mt-3 flex gap-2 text-sm"
                onSubmit={async (e) => {
                    e.preventDefault();

                    const res = await fetch('/report', {
                        method: 'POST',
                        body: JSON.stringify({ url }),
                        headers: { 'Content-Type': 'application/json' }
                    });
                    const data = await res.json();

                    if (res.ok) {
                        setMessage(data.message);
                        setError('');
                    } else {
                        setError(data.message);
                        setMessage('');
                    }

                    setUrl('');
                }}
            >
                <input
                    type="text"
                    className="rounded border border-black/25 px-3 py-2 grow"
                    placeholder="Enter dangerous URL here..."
                    value={url}
                    onChange={(e) => setUrl(e.target.value)}
                />
                <button className="bg-indigo-500 text-white rounded px-3 py-2 font-medium cursor-pointer">
                    Report URL
                </button>
            </form>
            {message && (
                <div className="rounded border border-blue-500 bg-blue-400/20 text-blue-600 px-2 py-2 mt-3 text-sm">
                    {message}
                </div>
            )}
            {error && (
                <div className="rounded border border-red-500 bg-red-500/20 text-red-600 px-2 py-2 mt-3 text-sm">
                    {error}
                </div>
            )}
        </>
    )
}
