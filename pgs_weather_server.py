import ssl
from urllib import request, parse
from utils.const_value import API, KEY, UNIT, LANGUAGE
from utils.helper import getLocation

gcontext = ssl.SSLContext(ssl.PROTOCOL_TLSv1)


def fetchWeather(location):
    params = parse.urlencode({
        'key': '',
        'location':'29.33:104.76',
        'language':'zh-Hans',
        'unit': 'c'
    })
    req = request.Request('{api}?{params}'.format(api=API, params=params))
    response = request.urlopen(req, context=gcontext).read().decode('UTF-8')
    return response


if __name__ == '__main__':
    location = getLocation()
    result = fetchWeather(location)
    print(result)
