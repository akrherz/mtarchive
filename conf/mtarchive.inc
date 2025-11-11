   ServerAdmin akrherz@iastate.edu
   DocumentRoot /isu/mtarchive/data

   Header set Access-Control-Allow-Origin "*"

RewriteEngine On
RewriteCond %{HTTP_USER_AGENT} PetalBot [NC]
RewriteRule . - [R=403,L]

  LogFormat "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combined
  LogFormat "%{X-Forwarded-For}i %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" proxy
  SetEnvIf X-Forwarded-For "^.*\..*\..*\..*" forwarded
  CustomLog "/var/log/httpd/mtarchive/access_log" combined env=!forwarded
  CustomLog "/var/log/httpd/mtarchive/access_log" proxy env=forwarded

   ErrorLog /var/log/httpd/mtarchive/error_log

  alias /era5land /mnt/era5land
  <Directory /mnt/era5land>
    Options Indexes FollowSymLinks
    IndexOptions NameWidth=*
    AllowOverride None
    Require all granted
  </Directory>

  <Directory /isu/mtarchive/data>
    Options Indexes FollowSymLinks
    IndexOptions NameWidth=*
    AllowOverride All
    Require all granted

    RewriteRule ^([12].*).zip$ /cgi-bin/cybox.py?fn=$1.zip [L]

  </Directory>

  <Directory /mnt/>
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

<Location /NSSFC>
  Options Indexes FollowSymLinks
  IndexOptions NameWidth=* FancyIndexing DescriptionWidth=*
</Location>
