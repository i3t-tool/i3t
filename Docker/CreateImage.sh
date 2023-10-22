echo "This script requires to login to docker hub"
echo "If you are not logged in, please do so:"
echo "docker login --username=cvutdcgi"

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"
cd "$SCRIPT_DIR"

docker build -t i3t-ci .
docker tag i3t-ci cvutdcgi/i3t-ci
docker push cvutdcgi/i3t-ci
