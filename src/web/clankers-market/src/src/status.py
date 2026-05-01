def uptime():
    try:
        with open("/proc/uptime", "r", encoding="utf-8") as handle:
            total_seconds = int(float(handle.read().split()[0]))
    except Exception as exc:
        return f"Uptime unavailable: {exc}"

    days = total_seconds // 86400
    hours = (total_seconds % 86400) // 3600
    minutes = (total_seconds % 3600) // 60
    seconds = total_seconds % 60
    return f"System uptime: {days}d {hours}h {minutes}m {seconds}s"
