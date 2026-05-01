#!/bin/bash

echo "Content-Type: text/html"

if [[ "$REQUEST_METHOD" != "POST" ]]; then
	echo "Status: 405 Method Not Allowed"
	echo "Allow: POST"
	echo
	echo "<html><body><h1>Method Not Allowed</h1></body></html>"
	exit 1
fi

if ! [[ "$CONTENT_TYPE" == application/x-www-form-urlencoded* ]]; then
	echo "Status: 400 Bad Request"
	echo
	echo "<html><body><h1>Bad Request</h1></body></html>"
	exit 1
fi

if [[ "$CONTENT_LENGTH" -ge 1000000 ]]; then
	echo "Status: 413 Payload Too Large"
	echo
	echo "<html><body><h1>Payload Too Large</h1></body></html>"
	exit 1
fi

# read form data
function urldecode() { 
	: "${1//+/ }"
	echo -e "${_//%/\\x}"
}

IFS= read -r -n "$CONTENT_LENGTH" request_body
declare -A form_data   
while IFS='=' read -r -d '&' key value && [[ -n "$key" ]]; do
    form_data["$(urldecode "$key")"]="$(urldecode "$value")"
done <<<"$request_body&"

function generate_content() {
	required_keys=(first_name last_name email phone resume school degree graduation_year q1 q2 q3 q4 q5)

	for key in "${required_keys[@]}"; do
		if [[ -z "${form_data[$key]}" ]]; then
			echo "Missing required field '$key'"
			return
		fi
	done

	if ! [[ "${form_data[email]}" =~ ^[a-zA-Z0-9_.+\-]+@[a-zA-Z0-9\-]+\.[a-zA-Z0-9\-]+$ ]]; then
		echo "Invalid email address"
		return
	fi

	if ! [[ "${form_data[phone]}" =~ ^\+?[0-9\s-]+$ ]]; then
		echo "Invalid phone number"
		return
	fi

	if [[ -n "${form_data[linkedin]}" ]] && ! [[ "${form_data[linkedin]}" =~ ^https?://(www\.)?linkedin\.com/in/[a-zA-Z0-9-]+/?$ ]]; then
		echo "Invalid LinkedIn URL"
		return
	fi

	if [[ -n "${form_data[github]}" ]] && ! [[ "${form_data[github]}" =~ ^https?://(www\.)?github\.com/[a-zA-Z0-9-]+/?$ ]]; then
		echo "Invalid GitHub URL"
		return
	fi

	if [[ -n "${form_data[website]}" ]] && ! [[ "${form_data[website]}" =~ ^https?://.*$ ]]; then
		echo "Invalid website URL"
		return
	fi

	if [[ "${form_data[graduation_year]}" -lt 2026 ]]; then
		echo "Invalid graduation year"
		return
	fi

	cat <<EOF
Thank you so much for your interest in COMPANY_NAME. We know that it takes time and energy to submit for a new role. 
Our recruiting team carefully reviewed your background and experience and, unfortunately, we won't be moving forward with your application at this time. <br><br>

We do encourage you to keep an eye for roles that may be a better match in the future. Thank you again for taking the time to apply!
EOF
}

echo "Status: 200 OK"
echo

cat <<EOF
<!doctype html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<title>Job Application</title>
		<link rel="stylesheet" href="/index.css" />
	</head>
	<body id="application-status">
	<main>
	<h1>Application Status</h1>
	<p>
	$(generate_content)
	</p>
	</main>
	</body>
</html>
EOF
