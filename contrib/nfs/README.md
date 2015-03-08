Deploy to NearlyFreeSpeech.NET
==============================

Simple fabfile to deploy bluster to NearlyFreeSpeech.NET servers.

You just need to create ``[Production] Apache 2.4 Generic`` site, take note of the ssh username and run:

    $ fab -u $your_ssh_user install config

At the end of the deploy, fabric will ask for some configuration data, make sure to fill it correctly. Only the Gist ID is required.

After this, you should go to the site settings page and add a new daemon with the following settings:

- Tag: bluster (or whatever tag name you want)
- Command Line: /home/protected/bluster.sh
- Working Directory: (no need to change)
- Run Daemon As: me

And add a new proxy:

- Protocol: fcgi
- Base URI: /
- Document Root: /
- Target Port: 8080
- Direct: (keep unchecked)

That's it, your app should start running shortly!
