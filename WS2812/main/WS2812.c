#include "WS2812.h"

static void obtain_time(void);

uint8_t buffer[1024];
uint32_t RGB[256];
uint8_t MODE=1;
bool led_on_off = true;
static const char *TAG="WS2812" ;
uint8_t brightness = 5; // 百分比

time_t now = 0;
struct tm timeinfo = { 0 };

uint32_t Dates[256]={0};

const uint8_t Code3x5[][15]={
    {1,1,1,1,1,
     1,0,0,0,1,
     1,1,1,1,1},//0
    {0,1,0,0,1,
     1,1,1,1,1,
     0,0,0,0,1},//1
    {1,0,1,1,1,
     1,0,1,0,1,
     1,1,1,0,1},//2
    {1,0,1,0,1,
     1,0,1,0,1,
     1,1,1,1,1},//3
    {1,1,1,0,0,
     0,0,1,0,0,
     1,1,1,1,1},//4
    {1,1,1,0,1,
     1,0,1,0,1,
     1,0,1,1,1},//5
    {1,1,1,1,1,
     1,0,1,0,1,
     1,0,1,1,1},//6
    {1,0,0,0,0,
     1,0,0,0,0,
     1,1,1,1,1},//7
    {1,1,1,1,1,
     1,0,1,0,1,
     1,1,1,1,1},//8
    {1,1,1,0,1,
     1,0,1,0,1,
     1,1,1,1,1},//9
    {0,0,0,0,0,
     0,1,0,1,0,
     0,0,0,0,0},//:
};

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
    //socket函数创建成功时的返回值是文件描述符，如果返回-1，则表示创建失败
    //socket三个参数分别是：地址类型（AF_INET表示IPv4）、套接字类型（SOCK_STREAM表示TCP）、协议（0表示默认协议）
    if (sockfd < 0) {//若返回值为-1，则表示创建失败，处理创建失败的情况
        perror("socket");//以socket：形式输出错误信息
        exit(EXIT_FAILURE);//退出
        return -1;
    }
    return sockfd;//返回创建成功的文件描述符
}

void connect_to_server(int sockfd) {
    struct sockaddr_in server_addr;//创建一个结构体变量，用于存储服务器的地址信息
    memset(&server_addr, 0, sizeof(server_addr));//清零初始化
    server_addr.sin_family = AF_INET;//设置地址族为IPv4
    server_addr.sin_port = htons(8080);//设置端口号为8080
    inet_pton(AF_INET, "192.168.1.104", &server_addr.sin_addr); //将点分十进制的IP地址转换为二进制形式，并存储在server_addr.sin_addr中
 
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {//连接服务器
        perror("connect");//以connect：形式输出错误信息
        closesocket(sockfd);//关闭套接字
        exit(EXIT_FAILURE);//退出
        vTaskDelay(6 / portTICK_PERIOD_MS);//延时6ms
    }
}

void Mode_0(led_strip_handle_t led_strip,int sockfd){
    switch (buffer[0])
    {
    case 0x00:
        led_on_off=false;send(sockfd, "WS2812_OFF", 10, 0);
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        break;
    case 0x11:
        led_on_off=true;send(sockfd, "WS2812_ON", 9, 0);
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        break;
    case 0x02:
        for (uint8_t i = 0; i < buffer[2]; i++)
        {
            RGB[(buffer[1]+i)*3]=buffer[3+i*3];
            RGB[(buffer[1]+i)*3+1]=buffer[3+i*3+1];
            RGB[(buffer[1]+i)*3+2]=buffer[3+i*3+2];
        }
        
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        
        send(sockfd, RGB,256*3, 0);
        break;
    case 0x08:
        brightness=buffer[1];
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        send(sockfd, "Brightness Change!",18 , 0);
        break;
    default:
        break;
    }
}

void Mode_1(led_strip_handle_t led_strip,int sockfd){
    //obtain_time();

    ESP_LOGI(TAG, "asdsadsads");
    time(&now);
    localtime_r(&now, &timeinfo);
    //char strftime_buf[64];
    //strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    //ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
        
    
    for (uint8_t j = 0; j < 5; j++)
    {
        Dates[225+j]=Code3x5[timeinfo.tm_sec%10][j]?TimeColor:0;
        Dates[234+j]=Code3x5[timeinfo.tm_sec%10][9-j]?TimeColor:0;
        Dates[241+j]=Code3x5[timeinfo.tm_sec%10][j+10]?TimeColor:0;

        Dates[202+j]=Code3x5[timeinfo.tm_sec/10][4-j]?TimeColor:0;
        Dates[209+j]=Code3x5[timeinfo.tm_sec/10][j+5]?TimeColor:0;
        Dates[218+j]=Code3x5[timeinfo.tm_sec/10][14-j]?TimeColor:0;


        Dates[193+j]=Code3x5[10][j+5]?TimeColor:0;

        Dates[170+j]=Code3x5[timeinfo.tm_min%10][4-j]?TimeColor:0;
        Dates[177+j]=Code3x5[timeinfo.tm_min%10][j+5]?TimeColor:0;
        Dates[186+j]=Code3x5[timeinfo.tm_min%10][14-j]?TimeColor:0;

        Dates[145+j]=Code3x5[timeinfo.tm_min/10][j]?TimeColor:0;
        Dates[154+j]=Code3x5[timeinfo.tm_min/10][9-j]?TimeColor:0;
        Dates[161+j]=Code3x5[timeinfo.tm_min/10][j+10]?TimeColor:0;

        Dates[138+j]=Code3x5[10][j+5]?TimeColor:0;

        Dates[113+j]=Code3x5[timeinfo.tm_hour%10][j]?TimeColor:0;
        Dates[122+j]=Code3x5[timeinfo.tm_hour%10][9-j]?TimeColor:0;
        Dates[129+j]=Code3x5[timeinfo.tm_hour%10][j+10]?TimeColor:0;

        Dates[90+j]=Code3x5[timeinfo.tm_hour/10][4-j]?TimeColor:0;
        Dates[97+j]=Code3x5[timeinfo.tm_hour/10][j+5]?TimeColor:0;
        Dates[106+j]=Code3x5[timeinfo.tm_hour/10][14-j]?TimeColor:0;
    }
    Dates[88]=(timeinfo.tm_wday==0)? WeekColor_1:WeekColor_0;
    Dates[103]=(timeinfo.tm_wday==0)?WeekColor_1:WeekColor_0;
    Dates[119]=(timeinfo.tm_wday==1)?WeekColor_1:WeekColor_0;
    Dates[120]=(timeinfo.tm_wday==1)?WeekColor_1:WeekColor_0;
    Dates[136]=(timeinfo.tm_wday==2)?WeekColor_1:WeekColor_0;
    Dates[151]=(timeinfo.tm_wday==2)?WeekColor_1:WeekColor_0;
    Dates[167]=(timeinfo.tm_wday==3)?WeekColor_1:WeekColor_0;
    Dates[168]=(timeinfo.tm_wday==3)?WeekColor_1:WeekColor_0;
    Dates[184]=(timeinfo.tm_wday==4)?WeekColor_1:WeekColor_0;
    Dates[199]=(timeinfo.tm_wday==4)?WeekColor_1:WeekColor_0;
    Dates[215]=(timeinfo.tm_wday==5)?WeekColor_1:WeekColor_0;
    Dates[216]=(timeinfo.tm_wday==5)?WeekColor_1:WeekColor_0;
    Dates[232]=(timeinfo.tm_wday==6)?WeekColor_1:WeekColor_0;
    Dates[247]=(timeinfo.tm_wday==6)?WeekColor_1:WeekColor_0;

    WS2812_Updata(led_strip,led_on_off,Dates,brightness);
    vTaskDelay(100 / portTICK_PERIOD_MS);//延时0.1s
    //const int deep_sleep_sec = 1;
    //esp_deep_sleep(1000000LL * deep_sleep_sec);

}


void app_main(void)
{
    ESP_LOGI(TAG,"ws2812 start");
    led_strip_handle_t led_strip = configure_led();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    int err;
    int sockfd = create_socket();//创建套接字
    connect_to_server(sockfd);//将套接字连接服务器
    vTaskDelay(1000 / portTICK_PERIOD_MS);//延时1s
    
    obtain_time();
    
    WS2812_Updata(led_strip,1,RGB,brightness);
    err = send(sockfd, "hello ESP32!", 12, 0);
    while (1)
    {

        //led_on_off = !led_on_off;
        //vTaskDelay(pdMS_TO_TICKS(500));
        vTaskDelay(1000 / portTICK_PERIOD_MS);//延时1s

        err = recv(sockfd, buffer, sizeof(buffer), 0x08);//通过套接字接收服务器发送的网络数据
        if(err > 0){
            
            switch (buffer[0])
            {
            case 0xEF:
                MODE=buffer[1];send(sockfd, "MODE CHANGE", 12, 0);
                break;
            default:
                switch (MODE)
                {
                case 0:
                    Mode_0(led_strip,sockfd);
                    break;
                case 1:
                    Mode_1(led_strip,sockfd);
                    break;
                default:
                    break;
                }
                break;
            }
            
        }
        else{
            switch (MODE)
                {
                case 0:
                    Mode_0(led_strip,sockfd);
                    break;
                case 1:
                    Mode_1(led_strip,sockfd);
                    break;
                default:
                    break;
                }
        }

    }
    

}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

static void obtain_time(void)
{
    /**
     * NTP server address could be aquired via DHCP,
     * see LWIP_DHCP_GET_NTP_SRV menuconfig option
     */
#ifdef LWIP_DHCP_GET_NTP_SRV
    sntp_servermode_dhcp(1);
#endif
 
    initialize_sntp();
 
    // wait for time to be set

    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);

    
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
    
    esp_netif_sntp_deinit();
    //ESP_ERROR_CHECK( example_disconnect() );
    /*
    for (uint8_t j = 0; j < 5; j++)
    {
        Dates[225+j]=Code3x5[timeinfo.tm_sec%10][j]?TimeColor:0;
        Dates[234+j]=Code3x5[timeinfo.tm_sec%10][j+5]?TimeColor:0;
        Dates[241+j]=Code3x5[timeinfo.tm_sec%10][j+10]?TimeColor:0;

        Dates[202+j]=Code3x5[timeinfo.tm_sec/10][4-j]?TimeColor:0;
        Dates[209+j]=Code3x5[timeinfo.tm_sec/10][4-j+5]?TimeColor:0;
        Dates[218+j]=Code3x5[timeinfo.tm_sec/10][4-j+10]?TimeColor:0;


        Dates[193+j]=Code3x5[10][j+5]?TimeColor:0;

        Dates[170+j]=Code3x5[timeinfo.tm_min%10][4-j]?TimeColor:0;
        Dates[177+j]=Code3x5[timeinfo.tm_min%10][4-j+5]?TimeColor:0;
        Dates[186+j]=Code3x5[timeinfo.tm_min%10][4-j+10]?TimeColor:0;

        Dates[145+j]=Code3x5[timeinfo.tm_min/10][j]?TimeColor:0;
        Dates[154+j]=Code3x5[timeinfo.tm_min/10][j+5]?TimeColor:0;
        Dates[161+j]=Code3x5[timeinfo.tm_min/10][j+10]?TimeColor:0;

        Dates[138+j]=Code3x5[10][j+5]?TimeColor:0;

        Dates[113+j]=Code3x5[timeinfo.tm_hour%10][j]?TimeColor:0;
        Dates[122+j]=Code3x5[timeinfo.tm_hour%10][j+5]?TimeColor:0;
        Dates[129+j]=Code3x5[timeinfo.tm_hour%10][j+10]?TimeColor:0;

        Dates[90+j]=Code3x5[timeinfo.tm_hour/10][4-j]?TimeColor:0;
        Dates[97+j]=Code3x5[timeinfo.tm_hour/10][4-j+5]?TimeColor:0;
        Dates[106+j]=Code3x5[timeinfo.tm_hour/10][4-j+10]?TimeColor:0;
    }*/
}
 
