import type { CSSProperties, ReactNode } from 'react';
import Anser, { type AnserJsonEntry } from 'anser';


function ansiToJSON(input: string, use_classes: boolean = false) {
    input = fixBackspace(input);
    return Anser.ansiToJson(input, {
        json: true,
        remove_empty: true,
        use_classes,
    });
}

function createClass(bundle: AnserJsonEntry) {
    let classNames: string = '';

    if (bundle.bg) classNames += `${bundle.bg}-bg `;
    if (bundle.fg) classNames += `${bundle.fg}-fg `;
    if (bundle.decoration) classNames += `ansi-${bundle.decoration} `;

    if (classNames === '') return;
    return classNames.trimEnd();
}

function createStyle(bundle: AnserJsonEntry) {
    const style: CSSProperties = {};
    if (bundle.bg) {
        style.backgroundColor = `rgb(${bundle.bg})`;
    }
    if (bundle.fg) {
        style.color = `rgb(${bundle.fg})`;
    }
    switch (bundle.decoration) {
        case 'bold':
            style.fontWeight = 'bold';
            break;
        case 'dim':
            style.opacity = '0.5';
            break;
        case 'italic':
            style.fontStyle = 'italic';
            break;
        case 'hidden':
            style.visibility = 'hidden';
            break;
        case 'strikethrough':
            style.textDecoration = 'line-through';
            break;
        case 'underline':
            style.textDecoration = 'underline';
            break;
        case 'blink':
            style.textDecoration = 'blink';
            break;
        default:
            break;
    }
    return style;
}


type AnserBundleProps = {
    linkify: boolean,
    useClasses: boolean,
    bundle: AnserJsonEntry
}

function AnserBundle(props: AnserBundleProps) {
    const style = props.useClasses ? undefined : createStyle(props.bundle);
    const className = props.useClasses ? createClass(props.bundle) : undefined;

    if (!props.linkify) return (
        <span
            style={style}
            className={className}
        >
            {props.bundle.content}
        </span>
    );

    const content: ReactNode[] = [];
    const linkRegex = /(\s|^)(https?:\/\/(?:www\.|(?!www))[^\s.]+\.\S{2,}|www\.\S+\.\S{2,})/g;

    let index = 0;
    let match: RegExpExecArray | null;
    while ((match = linkRegex.exec(props.bundle.content)) !== null) {
        const [, pre, url] = match;

        const startIndex = match.index + pre.length;
        if (startIndex > index) {
            content.push(props.bundle.content.substring(index, startIndex));
        }

        // Make sure the href we generate from the link is fully qualified. We assume http
        // if it starts with a www because many sites don't support https
        const href = url.startsWith("www.") ? `http://${url}` : url;
        content.push(
            <a key={index} href={href} target="_blank">
                {url}
            </a>
        );

        index = linkRegex.lastIndex;
    }

    if (index < props.bundle.content.length) {
        content.push(props.bundle.content.substring(index));
    }

    return (
        <span
            style={style}
            className={className}
        >
            {content}
        </span>
    );
}

type AnsiProps = {
    children?: string;
    linkify?: boolean;
    className?: string;
    useClasses?: boolean;
}

export default function Ansi(props: AnsiProps) {
    const useClasses = props.useClasses ?? false;
    const linkify = props.linkify ?? false;

    return (
        <pre className={props.className}>
            {ansiToJSON(props.children ?? '', useClasses).map((e, i) => (
                <AnserBundle
                    linkify={linkify}
                    useClasses={useClasses}
                    bundle={e}
                    key={i}
                />
            ))}
        </pre>
    );
}

// This is copied from the Jupyter Classic source code
// notebook/static/base/js/utils.js to handle \b in a way
// that is **compatible with Jupyter classic**.   One can
// argue that this behavior is questionable:
//   https://stackoverflow.com/questions/55440152/multiple-b-doesnt-work-as-expected-in-jupyter#
function fixBackspace(txt: string) {
    let tmp = txt;
    do {
        txt = tmp;
        // Cancel out anything-but-newline followed by backspace
        tmp = txt.replace(/[^\n]\x08/gm, "");
    } while (tmp.length < txt.length);
    return txt;
}
