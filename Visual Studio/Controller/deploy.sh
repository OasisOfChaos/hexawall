#!/bin/bash
"/mnt/c/Program Files/dotnet/dotnet.exe" publish -c Release

ssh root@runner 'systemctl stop kestrel-hexawall'

scp -r bin/Release/net6.0/publish/* root@runner:/srv/web/hexawall.nntw.nl/

ssh root@runner 'chown -R www-data:www-data /srv/web/hexawall.nntw.nl'

ssh root@runner 'systemctl daemon-reload'
ssh root@runner 'systemctl restart kestrel-hexawall'
