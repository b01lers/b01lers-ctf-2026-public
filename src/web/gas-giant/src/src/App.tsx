import { Route, Routes } from 'react-router';
import HomePage from './HomePage';
import NotebookPage from './NotebookPage';


export default function App() {
    return (
        <Routes>
            <Route index element={<HomePage />} />
            <Route path="/render" element={<NotebookPage />} />
        </Routes>
    )
}
