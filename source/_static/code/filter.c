#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <event.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <libmemcached/memcached.h>

FILE *f = NULL;
size_t t = 0;
char *buff = NULL;
size_t buff_size = 2000;
const char *file_name = "/tmp/access_log";
char    ip[17] = "\0",
        dim[2],
        username[17] = "\0",
        datetime[22],
        time_zone[7],
        method[4],
        url[100],
        http_version[9];
uint16_t    state_code;
memcached_st *memc = NULL;
memcached_return_t rc;
size_t value_len = 16;
size_t key_len;

static void
ev_read(int fd, short event, void *argv)
{
    if ((t = getline(&buff, &buff_size, stdin)) != -1) {

        fprintf(f, "%s", buff);
        fflush(f);

        // fprintf(stdout, "%zu\t%s", t, buff);
        sscanf(buff, "%s %s %s [%s %s \"%s %s %s %hu",
                                    ip,
                                    dim,
                                    username,
                                    datetime,
                                    time_zone,
                                    method,
                                    url,
                                    http_version,
                                    &state_code);
        struct in_addr ip_addr;
        inet_aton(ip, &ip_addr);

        char ret_url[50];
        sscanf(url, "%[^?]", ret_url);

        char key[60] = "\0";
        char *value;
        time_t t = time(NULL);
        time_t timestamp;
        size_t  count;
        struct tm m;
        int8_t active = 0;
        uint32_t flags = 0;

        sprintf(key, "%u_%s", ip_addr.s_addr, ret_url);
        key_len = strlen(key);

        fprintf(stdout, "Key: %s\n", key);
        if ((value = memcached_get(memc, key, key_len, &value_len, 0, &rc))
                                                                    != NULL) {
            // handle with errors.
            fprintf(stdout, "%s\n", value);
            sscanf(value, "%ld %zu", &timestamp, &count);
            fprintf(stdout, "%ld %zu %ld\n", timestamp, count, t - timestamp);

            if ((t - timestamp) >= 10 && (t - timestamp) < 30) {
                if (count > 30)
                    active = 1;
                else
                    active = 0;
            }else if ((t - timestamp) >= 30 && (t - timestamp) < 60) {
                if (count > 70)
                    active = 1;
                else
                    active = 0;
            }else{
                if (count > 130)
                    active = 1;
                else
                    // timeout
                    active = 2;
            }
            fprintf(stderr, "%s\nactive: %d\nKey: %s\n", memcached_strerror(memc, rc), active, key);

            if (active == 1) {
                // insert into black list
                //
                if ((rc = memcached_delete(memc, key, key_len, 0)) != MEMCACHED_SUCCESS) {
                    fprintf(stderr, "%s\nactive: %d\nkey: %s\n", memcached_strerror(memc, rc), active, key);
                    return;
                }
            }else if (active == 2) {
                if ((rc = memcached_delete(memc, key, key_len, 0)) != MEMCACHED_SUCCESS) {
                    fprintf(stderr, "%s\nactive: %d\nKey: %s\n", memcached_strerror(memc, rc), active, key);
                    return;
                }
            }else if (active == 0) {
                // update
                count++;
                fprintf(stdout, "B: %ld %zu\n", timestamp, count);
                sprintf(value, "%ld %zu", timestamp, count);
                fprintf(stdout, "A: %ld %zu\n", timestamp, count);
                if ((rc = memcached_replace(memc, key, strlen(key), value, strlen(value), 181, flags)) != MEMCACHED_SUCCESS) {
                    fprintf(stderr, "%s\nactive: %d\nKey: %s\n", memcached_strerror(memc, rc), active, key);
                    return;
                }else
                    fprintf(stdout, "update successfully! %zu\n", count);
            }else{
                fprintf(stdout,
                        "the state of key \"%s\" occure errors: %u!\n",
                        key, active);
            }
        }

        value = calloc(sizeof(char), 16);

        strptime(datetime, "%d/%b/%Y:%T", &m);
        t = mktime(&m);

        sprintf(value, "%lu %u", t, 1);
        value_len = strlen(value);

        if ((rc =
                memcached_add(memc, key, key_len, value, value_len, 181, flags))
                                                        != MEMCACHED_SUCCESS) {
            fprintf(stderr, "New: %s\nLen: %zu\n", memcached_strerror(memc, rc), key_len);
            return;
        }else
            fprintf(stdout, "add successfully!\n");

        free(value);
    }else
        exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
    struct event_base *base;
    struct event *ev;
    int fn = 0;

    const char *config_string = "--SERVER=localhost";
    memc = memcached(config_string, strlen(config_string));


    fn = open("/dev/stdin", O_RDONLY | O_NONBLOCK, 0);
    f = fopen(file_name, "a+");

    base = event_base_new();
    ev = event_new(base, STDIN_FILENO, EV_READ|EV_PERSIST, ev_read, NULL);
    event_add(ev, NULL);
    event_base_dispatch(base);

    fclose(f);
    memcached_free(memc);
    return 0;
}
