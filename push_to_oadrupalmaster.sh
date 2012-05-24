rsync -av --exclude '.git' /Users/rypalmer/Sites/oahealthcare/ /Users/rypalmer/Sites/dev.portal.oahealthcare.com.staging/dev.portal.oahealthcare.com/sites/all/modules/oahealthportal
rsync -av /Users/rypalmer/Sites/oahealthcare/wsdl/oatest/ /Users/rypalmer/Sites/dev.portal.oahealthcare.com.staging/dev.portal.oahealthcare.com/sites/all/modules/oahealthportal/wsdl
rsync -rv /Users/rypalmer/Sites/dev.portal.oahealthcare.com.staging/dev.portal.oahealthcare.com/sites/all/ oadrupalmaster:/var/www/html/dev.portal.oahealthcare.com/sites/all/
