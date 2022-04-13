   ServerAdmin akrherz@iastate.edu
   DocumentRoot /isu/mtarchive/data

RewriteEngine On
RewriteCond %{HTTP_USER_AGENT} PetalBot [NC]
RewriteRule . - [R=403,L]

  LogFormat "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combined
  LogFormat "%{X-Forwarded-For}i %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" proxy
  SetEnvIf X-Forwarded-For "^.*\..*\..*\..*" forwarded
  CustomLog "/var/log/httpd/mtarchive/access_log" combined env=!forwarded
  CustomLog "/var/log/httpd/mtarchive/access_log" proxy env=forwarded

   ErrorLog /var/log/httpd/mtarchive/error_log

  <Directory /isu/mtarchive/data>
    Options Indexes FollowSymLinks
    IndexOptions NameWidth=*
    AllowOverride None
    Require all granted

    RewriteRule ^(.*).zip$ /cgi-bin/cybox.py?fn=$1.zip [L]

  </Directory>

  <Directory /mnt/mtarchive8/data>
    Options Indexes FollowSymLinks
    IndexOptions NameWidth=*
    AllowOverride None
    Require all granted

    RewriteRule ^(.*).zip$ /cgi-bin/cybox.py?fn=$1.zip [L]

  </Directory>

  <LocationMatch /nldn/>
     SetEnvIf X-Forwarded-For "^10\.64\..*\..*" forwarded2
     SetEnvIf X-Forwarded-For "^129\.186\..*\..*" forwarded3
     Require env forwarded2
     Require env forwarded3
  </LocationMatch>

  ProxyPreserveHost On
  ProxyPass /thredds http://localhost:8080/thredds
  ProxyPassReverse /thredds http://localhost:8080/thredds

Alias /cgi-bin/ "/opt/mtarchive/cgi-bin/"
<Directory "/opt/mtarchive/cgi-bin">
    AllowOverride All
    Options FollowSymLinks
    Require all granted
    AddHandler cgi-script .py
    Options +ExecCGI
</Directory>