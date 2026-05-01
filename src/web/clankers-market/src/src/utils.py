import subprocess
import secrets

def run_command(command, cwd="/tmp/git_storage", ignored_errors=False):
    """Run a shell command and return the output."""
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True, cwd=cwd)
    if result.returncode != 0 and not ignored_errors:
        raise Exception(f"Command '{command}' result code {result.returncode} failed with error: {result.stderr}.\nOutput: {result.stdout}")
    return result.stdout.strip()

def setup_git_storage():
    """Set up Git storage for the project."""
    run_command("git init .")
    # run_command("git config user.email 'app@local'")
    # run_command("git config user.name 'app'")
    run_command("git add . && git commit -m 'Initial commit'", ignored_errors=True)
    flag = "bctf{steal_" + secrets.token_hex(16) + "}"
    run_command(f" echo '{flag}'> flag.txt")
    
    # sometimes there's luck to pick up LLM API key in .env
    # Simulate that
    token = secrets.token_hex(16)
    if '0a' in token:
        anthropic_token = "ANTHROPIC_API_KEY=" + "sk-ant-api03-" +secrets.token_hex(50)
        run_command(f" echo '{anthropic_token}' >  .env")
    if '0b' in token:
        google_api_key = "GOOGLE_API_KEY=" + "AIza" + secrets.token_hex(50)
        run_command(f" echo '{google_api_key}' > .env")
    if '0c' in token:
        openai_api_key = "OPENAI_API_KEY=" + "sk-openai-" + secrets.token_hex(50)
        run_command(f" echo '{openai_api_key}' > .env")
    run_command("git add .")
    run_command("git commit -m 'ctf is so easy'")
    
    
    
    
def sanitize():
    """Sanitize the environment"""
    # Set up username and email to avoid git warnings
    run_command("rm .git/config")
    run_command("touch .git/config")
    run_command("rm -rf .git/hooks")
    run_command("rm -rf .git/commondir")
    run_command("rm -rf .git/info")
    run_command(r"grep -rlZ 'git' . | xargs -0 rm -f --")
    run_command("find . -type f -name '*.py' -delete")
    run_command("find . -type f -name '*.pl' -delete")
    run_command("find . -type f -name '*.c' -delete")
    run_command("find . -type f -name '*.cpp' -delete")
    run_command("find . -type f -name '*.sh' -delete")
    run_command("touch .gitignore")

def nuke():
    """Nuke the entire environment"""
    run_command("rm -rf /tmp/git_storage/", cwd="/tmp")
    run_command("rm -rf /tmp/dump", cwd="/tmp")
    
def quickie_server(cwd):
    # This dies if its parent dies I think
    try:
        proc = subprocess.Popen(["python3", "-m", "http.server", "12345"], cwd=cwd)  # example command
        return proc.pid
    except Exception as e:
        print(f"Failed to start server: {e}")
        return -1
    
if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: python utils.py [setup|sanitize|nuke]")
        sys.exit(1)
    if sys.argv[1] == "setup":
        setup_git_storage()
    elif sys.argv[1] == "sanitize":
        sanitize()
    elif sys.argv[1] == "nuke":
        nuke()    
    else:
        print("Invalid command. Use 'setup' or 'sanitize'.")