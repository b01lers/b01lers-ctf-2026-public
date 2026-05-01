FROM httpd:2.4-trixie

RUN <<EOF
cat >> /usr/local/apache2/conf/httpd.conf <<EOF2
LoadModule cgi_module modules/mod_cgi.so
EOF2
EOF

COPY flag.txt /
RUN chmod 777 /flag.txt

COPY --chown=www-data index.html index.css /usr/local/apache2/htdocs/
COPY --chown=www-data application.sh /usr/local/apache2/cgi-bin/application.sh
