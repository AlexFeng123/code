
# simple usage message
function doUsage {
  echo "usage: . ./getToken.sh <ip address>"
}

#if no arguement supplied show usage and exit
if [ -z "$1" ]; 
then
  doUsage
else
  export target=$1
  export hdr1="Content-Type:application/json;ext=nn"
  body='{"in":{"pswd":"CHGME.1a","un":"admin"}}'
  uri="https://$target/auth?actn=lgin"
  export token="$(curl -k --silent -i -X POST -H "$hdr1" -d $body $uri | grep X-Auth-Token: | sed -e 's/X-Auth-Token: //g')"
  echo $target
  echo $token
fi


