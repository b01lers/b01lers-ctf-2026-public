import { useState } from 'react';

// Components
import StyledNotebook from './StyledNotebook';
import SecuritySection from './SecuritySection';

// Utils
import type { Notebook } from './lib/types';
import { notebookSchema } from './NotebookPage';


export default function HomePage() {
    const [data, setData] = useState<{ encoded: string | null, error: string | null }>({ encoded: null, error: null });

    return (
        <div className="container py-20">
            <h1 className="text-4xl font-bold mb-4">
                b01lerLite
            </h1>
            <p className="mb-4">
                <strong>b01lerLite</strong> is a lightweight™, online host for Jupyter Notebook sharing.
                Try uploading a file, or view a demo below:
            </p>

            <div className="border border-black/20 rounded-lg shadow-md mb-6">
                <StyledNotebook
                    notebook={lorenz}
                    className="py-6 h-[36rem] overflow-y-auto gap-4 text-sm"
                />
            </div>

            <input
                type="file"
                className="file:border file:border-black/10 file:bg-gray-100 text-sm file:rounded-full file:px-2.5 file:py-0.5 file:cursor-pointer file:font-medium"
                accept=".ipynb"
                onChange={(e) => e.target.files?.item(0)?.text().then(s => setData(parseFileContent(s)))}
            />
            {data.encoded ? (
                <div className="break-words border border-blue-500 bg-blue-400/20 text-blue-600 px-3 py-2 mt-3 rounded">
                    <h4 className="font-semibold text-sm mb-1">
                        Generated shareable link:
                    </h4>
                    <a
                        target="_blank"
                        rel="noopener noreferrer"
                        href={`/render?d=${data.encoded}`}
                        className="text-xs text-blue-600 hover:underline line-clamp-2"
                    >
                        {window.location.href}render?d={data.encoded}
                    </a>
                </div>
            ) : data.error && (
                <div className="border border-red-500 rounded bg-red-500/20 text-red-600 px-2 py-2 mt-3 text-sm">
                    {data.error}
                </div>
            )}

            <SecuritySection />
        </div>
    )
}

function parseFileContent(content: string) {
    if (!content) return { encoded: null, error: null };

    try {
        // Verify notebook schema is correct
        const data = notebookSchema.parse(JSON.parse(content));

        // Make sure the URL isn't too long; we re-stringify the parsed data instead of just
        // using the string `content` to compress the JSON a bit
        const encoded = btoa(JSON.stringify(data));
        console.log(encoded.length)
        if (encoded.length > 8000) return { encoded: null, error: 'Your file is too large to be encoded.' };

        return { encoded, error: null };
    } catch (e) {
        return { encoded: null, error: e instanceof Error ? e.message : ':(' };
    }
}

const lorenz: Notebook = {
    "metadata": {},
    "nbformat_minor": 4,
    "nbformat": 4,
    "cells": [
        {
            "cell_type": "markdown",
            "source": "# The Lorenz Differential Equations",
            "metadata": {}
        },
        {
            "cell_type": "markdown",
            "source": "Before we start, we import some preliminary libraries.",
            "metadata": {}
        },
        {
            "cell_type": "code",
            "source": "import numpy as np\nfrom matplotlib import pyplot as plt\nfrom scipy import integrate",
            "metadata": {},
            "outputs": [],
            "execution_count": 2
        },
        {
            "cell_type": "markdown",
            "source": "We will also define the actual solver and plotting routine.",
            "metadata": {}
        },
        {
            "cell_type": "code",
            "source": "def solve_lorenz(sigma=10.0, beta=8./3, rho=28.0):\n    \"\"\"Plot a solution to the Lorenz differential equations.\"\"\"\n\n    max_time = 4.0\n    N = 30\n\n    fig = plt.figure(1)\n    ax = fig.add_axes([0, 0, 1, 1], projection='3d')\n    ax.axis('off')\n\n    # prepare the axes limits\n    ax.set_xlim((-25, 25))\n    ax.set_ylim((-35, 35))\n    ax.set_zlim((5, 55))\n\n    def lorenz_deriv(x_y_z, t0, sigma=sigma, beta=beta, rho=rho):\n        \"\"\"Compute the time-derivative of a Lorenz system.\"\"\"\n        x, y, z = x_y_z\n        return [sigma * (y - x), x * (rho - z) - y, x * y - beta * z]\n\n    # Choose random starting points, uniformly distributed from -15 to 15\n    np.random.seed(1)\n    x0 = -15 + 30 * np.random.random((N, 3))\n\n    # Solve for the trajectories\n    t = np.linspace(0, max_time, int(250*max_time))\n    x_t = np.asarray([integrate.odeint(lorenz_deriv, x0i, t)\n                      for x0i in x0])\n\n    # choose a different color for each trajectory\n    colors = plt.cm.viridis(np.linspace(0, 1, N))\n\n    for i in range(N):\n        x, y, z = x_t[i,:,:].T\n        lines = ax.plot(x, y, z, '-', c=colors[i])\n        plt.setp(lines, linewidth=2)\n    angle = 104\n    ax.view_init(30, angle)\n    plt.show()\n\n    return t, x_t",
            "metadata": {},
            "outputs": [],
            "execution_count": 3
        },
        {
            "cell_type": "markdown",
            "source": "We explore the Lorenz system of differential equations:\n\n$$\n\\begin{aligned}\n\\dot{x} & = \\sigma(y-x) \\\\\n\\dot{y} & = \\rho x - y - xz \\\\\n\\dot{z} & = -\\beta z + xy\n\\end{aligned}\n$$\n\nLet's change ($\\sigma$, $\\beta$, $\\rho$) and examine the trajectories.",
            "metadata": {}
        },
        {
            "cell_type": "code",
            "source": "t, x_t = solve_lorenz(sigma=10.0, beta=8.0 / 3, rho=28.0)",
            "metadata": {},
            "outputs": [],
            "execution_count": 4
        },
        {
            "cell_type": "markdown",
            "source": "For the default set of parameters, we see the trajectories swirling around two points, called attractors. ",
            "metadata": {}
        },
        {
            "cell_type": "markdown",
            "source": "After interacting with the system, we can take the result and perform further computations. In this case, we compute the average positions in \\\\(x\\\\), \\\\(y\\\\) and \\\\(z\\\\).",
            "metadata": {}
        },
        {
            "cell_type": "code",
            "source": "xyz_avg = x_t.mean(axis=1)",
            "metadata": {},
            "outputs": [],
            "execution_count": 5
        },
        {
            "cell_type": "code",
            "source": "xyz_avg.shape",
            "metadata": {},
            "outputs": [],
            "execution_count": 6
        },
        {
            "cell_type": "markdown",
            "source": "Creating histograms of the average positions (across different trajectories) show that, on average, the trajectories swirl about the attractors.",
            "metadata": {}
        },
        {
            "cell_type": "code",
            "source": "from matplotlib import pyplot as plt\n%matplotlib inline",
            "metadata": {},
            "outputs": [],
            "execution_count": 7
        },
        {
            "cell_type": "code",
            "source": "plt.hist(xyz_avg[:,0])\nplt.title('Average $x(t)$');",
            "metadata": {},
            "outputs": [],
            "execution_count": 9
        },
        {
            "cell_type": "code",
            "source": "",
            "metadata": {},
            "outputs": [],
            "execution_count": null
        }
    ]
}
