FROM ubuntu:20.04

RUN set -x \
    && apt-get update \
    && apt-get install --no-install-recommends --no-install-suggests -y python3 ca-certificates curl \
    && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh \
    && arduino-cli config init \
    && sed -i 's/additional_urls:\ \[\]/additional_urls:\ \[https:\/\/arduino.esp8266.com\/stable\/package_esp8266com_index\.json\]/' /root/.arduino15/arduino-cli.yaml \
    && arduino-cli core update-index \
    && arduino-cli core install esp8266:esp8266


