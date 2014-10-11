# Foxriver

## 简介

规则性的中文语义分析

## 用法

```
mkdir build
cd build
cmake ..
make
./bin/foxriver -p 14311
```

```
curl "http://127.0.0.1:14311/?sentence=我明天要去上海，帮我定个浦东新区的酒店"
```

```
{"time": "20141012", "cityName": "上海", "cityId": "2", "cityCode": "SHA", "locationId": "119", "locationName":"浦东新区", "locationCityId":"2"}
```

