#define MODULE_NAME    "ntp.so"

#define NTP_TIMESTAMP_DELTA 2208988800ull

#define NTP_OK              0
#define NTP_ERROR_SOCKET    1
#define NTP_ERROR_CONNECT   2

typedef struct
{
    uint8_t li_vn_mode;

    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;

    uint32_t rootDelay;
    uint32_t rootDispersion;
    uint32_t refId;

    uint32_t refTm_s;
    uint32_t refTm_f;

    uint32_t origTm_s;
    uint32_t origTm_f;

    uint32_t rxTm_s;
    uint32_t rxTm_f;

    uint32_t txTm_s;
    uint32_t txTm_f;

} ntp_packet;

ntp_packet create_ntp_packet(void);
int get_diff(char *host, int port, int **diff);
int calc_diff(time_t t1, time_t t2);