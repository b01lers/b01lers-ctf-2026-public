from functools import wraps
from flask import Flask, redirect, render_template, request, session, url_for
from utils import *
import os
import secrets
from threading import Lock
import requests

app = Flask(__name__)
app.secret_key = secrets.token_hex(32)

WORKDIR = "/tmp/git_storage"
USERS = {}

GLOBAL_LOCK = Lock()

def setup():
    # Make sure the working directory exists
    run_command(f"mkdir -p {WORKDIR}", cwd="/tmp")


def login_required(func):
    @wraps(func)
    def wrapped(*args, **kwargs):
        if "username" not in session:
            return redirect(url_for("login"))
        return func(*args, **kwargs)
    return wrapped


@app.after_request
def add_static_cache_headers(response):
    if request.path.startswith("/static/"):
        response.headers["Cache-Control"] = "public, max-age=3600"
    return response

@app.route('/')
def index():
    return render_template("index.html", username=session.get("username"))


@app.route("/register", methods=["GET", "POST"])
def register():
    error = None
    if request.method == "POST":
        username = request.form.get("username", "").strip()
        password = request.form.get("password", "")

        if not username or not password:
            error = "Username and password are required."
        elif username in USERS:
            error = "Username already exists."
        else:
            USERS[username] = password
            session["username"] = username
            return redirect(url_for("listing"))

    return render_template("register.html", error=error)


@app.route("/login", methods=["GET", "POST"])
def login():
    error = None
    if request.method == "POST":
        username = request.form.get("username", "").strip()
        password = request.form.get("password", "")

        if USERS.get(username) == password:
            session["username"] = username
            return redirect(url_for("listing"))

        error = "Invalid username or password."

    return render_template("login.html", error=error)


@app.route("/logout", methods=["POST"])
def logout():
    session.pop("username", None)
    return redirect(url_for("index"))


@app.route("/listing")
@login_required
def listing():
    return render_template("listing.html", username=session.get("username"))


@app.route("/server-status")
def server_status():
    import status
    return status.uptime(), 200

@app.route('/clanker-feature', methods=['GET', 'POST'])
@login_required
def test():
    if request.method == "GET":
        return render_template("clanker_feature.html", username=session.get("username"))

    with GLOBAL_LOCK:
        uploaded_files = request.files.getlist('file')
        
        if not uploaded_files:
            return render_template(
                "clanker_feature.html",
                username=session.get("username"),
                error="No file part"
            ), 400

        if len(uploaded_files) > 2:
            return render_template(
                "clanker_feature.html",
                username=session.get("username"),
                error="Maximum 2 files allowed"
            ), 400

        # Validate all filenames before touching the filesystem
        validated = []
        for file in uploaded_files:
            if file.filename == '':
                return render_template(
                    "clanker_feature.html",
                    username=session.get("username"),
                    error="No selected file"
                ), 400

            file_path = os.path.join(WORKDIR, file.filename)
            normalized_path = os.path.abspath(file_path)

            if not normalized_path.startswith(WORKDIR + os.sep):
                return render_template(
                    "clanker_feature.html",
                    username=session.get("username"),
                    error="That vuln in the big 26?"
                ), 403

            validated.append((file, normalized_path))

        # Setup
        setup()
        setup_git_storage()

        # Save all validated files
        for file, normalized_path in validated:
            os.makedirs(os.path.dirname(normalized_path), exist_ok=True)
            file.save(normalized_path)

        # Sanitize the environment
        sanitize()

        pid = quickie_server(WORKDIR)
        if pid == -1:
            return render_template(
                "clanker_feature.html",
                username=session.get("username"),
                error="Failed to start server"
            ), 500
        
        # run git-dumper
        try:
            run_command("mkdir /tmp/dump")
            run_command("git-dumper http://localhost:12345 /tmp/dump")
        except Exception as e:
            # Cleanup
            nuke()
            run_command(f"kill -9 {pid}", ignored_errors=True, cwd="/tmp")
            return render_template(
                "clanker_feature.html",
                username=session.get("username"),
                error=f"Failed to run git-dumper: {e}"
            ), 500
        
        try:
            with open("/tmp/dump/flag.txt", "r") as f:
                flag = f.read().strip()
            # run a grep command for sk- and AIza to simulate the LLM API key leak
            ant_leak = run_command(r"grep -rlZ 'sk-ant' /tmp/dump | xargs -0 cat", ignored_errors=True)
            openai_leak = run_command(r"grep -rlZ 'sk-openai' /tmp/dump | xargs -0 cat", ignored_errors=True)
            google_leak = run_command(r"grep -rlZ 'AIza' /tmp/dump | xargs -0 cat", ignored_errors=True)
        except Exception as e:
            # Cleanup
            nuke()
            run_command(f"kill -9 {pid}", ignored_errors=True)
            return render_template(
                "clanker_feature.html",
                username=session.get("username"),
                error=f"Failed to read flag: {e}"
            ), 500
        
        # Cleanup
        nuke()
        run_command(f"kill -9 {pid}", ignored_errors=True, cwd="/tmp")
        return render_template(
            "clanker_feature.html",
            username=session.get("username"),
            success=True,
            uploaded_filename=file.filename,
            flag=flag,
            ant_leak=ant_leak,
            openai_leak=openai_leak,
            google_leak=google_leak
        ), 200

if __name__ == '__main__':
    setup()
    app.run(host='0.0.0.0', port=5000)
    