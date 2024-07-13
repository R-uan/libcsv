FROM --platform=linux/amd64 alpine:3.20.1

WORKDIR /app

COPY . .

RUN ./build.sh

ENV LD_LIBRARY_PATH=/app

CMD ["/app/main"]
