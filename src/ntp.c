#include "sysinc.h"
#include "module.h"
#include "common.h"
#include "log.h"
#include "zbxjson.h"
#include <locale.h>
#include "ntp.h"

ntp_packet create_ntp_packet(void)
{
    ntp_packet packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memset(&packet, 0, sizeof(ntp_packet));
    *((char *)&packet + 0) = 0x1b;
    return packet;
}

int calc_diff(time_t t1, time_t t2)
{
    int diff;
    diff = (t1 - t2);
    return diff;
}

int get_diff(char *host, int port, int **diff)
{
    const char *__function_name = "get_diff";
    int sock;
    int ret;
    struct sockaddr_in addr;
    time_t now;

    now = time(NULL);

    zabbix_log(LOG_LEVEL_TRACE,
                   "Module: %s, function: %s - now: %lu (%s:%d)",
                   MODULE_NAME, __function_name, now, __FILE__, __LINE__);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0)
    {
        zabbix_log(LOG_LEVEL_TRACE,
                   "Module: %s, function: %s - cannot create socket (%s:%d)",
                   MODULE_NAME, __function_name, __FILE__, __LINE__);
        return NTP_ERROR_SOCKET;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);

    ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        zabbix_log(LOG_LEVEL_TRACE,
                   "Module: %s, function: %s - cannot connect to host %s:%d (%s:%d)",
                   MODULE_NAME, __function_name, host, port, __FILE__, __LINE__);
        return NTP_ERROR_CONNECT;
    }

    ntp_packet packet = create_ntp_packet();

    ret = write(sock, (char*)&packet, sizeof(ntp_packet));
    if (ret < 0)
    {
        zabbix_log(LOG_LEVEL_TRACE,
                   "Module: %s, function: %s - cannot send NTP packet (%s:%d)",
                   MODULE_NAME, __function_name, __FILE__, __LINE__);
        return 1;
    }

    ret = read(sock, (char*)&packet, sizeof(ntp_packet));
    if (ret < 0)
    {
        zabbix_log(LOG_LEVEL_TRACE,
                   "Module: %s, function: %s - cannot read NTP packet (%s:%d)",
                   MODULE_NAME, __function_name, __FILE__, __LINE__);
        return 1;
    }

    packet.txTm_s = ntohl(packet.txTm_s);

    time_t txTm = (time_t) (packet.txTm_s - NTP_TIMESTAMP_DELTA);
    *diff = calc_diff(now, txTm);

    return NTP_OK;
}
