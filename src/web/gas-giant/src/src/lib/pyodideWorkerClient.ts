let lastId = 1;
export function getId() {
    return lastId++;
}

export async function receiveType(worker: Worker, type: string) {
    return new Promise<void>((res) => {
        worker.addEventListener('message', function listener(e) {
            if (e.data.output_type !== type) return;
            worker.removeEventListener('message', listener);
            res();
        })
    })
}
