import express from 'express';
import path from 'node:path';
import bodyParser from 'body-parser';
import { visit } from './bot.js';


const app = express();

app.use(bodyParser.json());
app.use(express.static(path.join(process.cwd(), 'dist')));

app.post('/report', async (req, res) => {
    const url = req.body.url;
    if (!url.startsWith('http://') && !url.startsWith('https://'))
        return void res.status(400).send({ message: 'Invalid URL protocol.' });

    void visit(url).catch((e) => console.log('Visit failed:', e));
    res.status(200).send({ message: 'The admin is visiting your URL.' })
})

app.get('*any', (req, res) => {
    res.sendFile(path.join(process.cwd(), 'dist', 'index.html'));
});

app.listen(3000, () => {
    console.log('Started server on port 3000');
});
