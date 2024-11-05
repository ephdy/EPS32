#include "TFT_ILI9341.h"

spi_device_handle_t spi;

void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    gpio_set_level(PIN_NUM_D, dc);
}

void spi_init(){
    
    for (int i = 0; i < 1000; i ++)
	{
		for (int j = 0; j < 1000; j ++);
	}

    
    spi_bus_config_t bus_cfg={
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092,
    };

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 10 * 1000 * 1000,     //Clock out at 10 MHz
        .mode = 0,                              //SPI mode 0
        .spics_io_num = PIN_NUM_CS,             //CS pin
        .queue_size = 7,                        //We want to be able to queue 7 transactions at a time
        .pre_cb = lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
    };

    ESP_ERROR_CHECK(spi_bus_initialize(TFT_HOST, &bus_cfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(TFT_HOST,&dev_cfg, &spi));

    gpio_config_t dc_cfg={
        .pin_bit_mask=(1<<GPIO_NUM_2),
        .pull_up_en=GPIO_PULLDOWN_DISABLE,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .intr_type=GPIO_INTR_DISABLE,
    };
    gpio_config(&dc_cfg);
    dc_cfg.pin_bit_mask=(1<<GPIO_NUM_4);
    gpio_config(&dc_cfg);
    dc_cfg.pin_bit_mask=(1<<GPIO_NUM_19);
    gpio_config(&dc_cfg);
}

void Delay(){
    for (int i = 0; i < 1000; i ++)
	{
		for (int j = 0; j < 1000; j ++);
	}
}


void tft_reset(){
    RES_ON;
    Delay();
    RES_OFF;
}

void tft_write_byte(uint8_t chr){
    spi_transaction_t trans_cfg; //定义数据结构体
    memset(&trans_cfg, 0, sizeof(trans_cfg));  //为数据结构体分配内存
    trans_cfg.cmd = 0x9F; //因为是固定内存地址，使用的是nterface_config的配置，也就是8bit cmd，0bit address
    trans_cfg.length = 1 * 8;  //要发送或者接收的数据的长度，不算前面的cmd/address/dummy的长度
    trans_cfg.tx_buffer =&chr;  //发送没有指定内部空间，使用的是外部区域，因此要自己指定
    trans_cfg.rx_buffer = NULL; //接收定义了SPI_TRANS_USE_RXDATA，使用的是内部空间。
    trans_cfg.flags = SPI_TRANS_USE_RXDATA;

    spi_device_polling_transmit(spi, &trans_cfg);
}

void tft_write_bytes(uint8_t chr[],uint8_t n){
    spi_transaction_t trans_cfg; //定义数据结构体
    memset(&trans_cfg, 0, sizeof(trans_cfg));  //为数据结构体分配内存
    trans_cfg.cmd = 0x9F; //因为是固定内存地址，使用的是nterface_config的配置，也就是8bit cmd，0bit address
    trans_cfg.length = n * 8;  //要发送或者接收的数据的长度，不算前面的cmd/address/dummy的长度
    trans_cfg.tx_buffer =chr;  //发送没有指定内部空间，使用的是外部区域，因此要自己指定
    trans_cfg.rx_buffer = NULL; //接收定义了SPI_TRANS_USE_RXDATA，使用的是内部空间。
    trans_cfg.flags = SPI_TRANS_USE_RXDATA;

    spi_device_polling_transmit(spi, &trans_cfg);
}

void tft_init(){
    spi_init();

    tft_reset();

    Delay();
    BLK_ON;
    Delay();
    tft_write_cmd(0x11);//退出睡眠模式
    Delay();

    tft_write_cmd(0XCF);//电源控制
    tft_write_data(0X00);
    tft_write_data(0XC1);//开启节电操作
    tft_write_data(0X30);//防静电保护

    tft_write_cmd(0XED);//开机顺序控制
    tft_write_data(0X64);//软启动保持两帧
    tft_write_data(0X03);
    tft_write_data(0X12);
    tft_write_data(0X81);//使能DDVDH_ENH

    tft_write_cmd(0XE8);//驱动器定时控制A
    tft_write_data(0X85);//NOW
    tft_write_data(0X00);//门驱动不重叠定时控制，CR振荡器定时控制
    tft_write_data(0X79);//不使用pre-charge定时控制

    tft_write_cmd(0XCB);//电压控制设定
    tft_write_data(0X39);//默认
    tft_write_data(0X2C);//默认
    tft_write_data(0X00);//默认
    tft_write_data(0X34);//默认
    tft_write_data(0X02);//DDVDH

    tft_write_cmd(0XF7);//泵比控制
    tft_write_data(0X20);//显示反转关闭
    //tft_write_data(0X20);//显示反转开启
    
    tft_write_cmd(0XEA);
    tft_write_data(0X00);
    tft_write_data(0X00);

    tft_write_cmd(0XC0);
    tft_write_data(0X1D);

    tft_write_cmd(0XC1);
    tft_write_data(0X12);

    tft_write_cmd(0XC5);
    tft_write_data(0X33);
    tft_write_data(0X3F);

    tft_write_cmd(0XC7);
    tft_write_data(0X92);

    tft_write_cmd(0X3A);
    tft_write_data(0X55);

    tft_write_cmd(0X36);//Memory Access Control
    tft_write_data(0X08);

    tft_write_cmd(0XB1);
    tft_write_data(0X00);
    tft_write_data(0X12);

    tft_write_cmd(0XB6);
    tft_write_data(0X0A);
    tft_write_data(0XA2);

    tft_write_cmd(0X44);
    tft_write_data(0X02);

    tft_write_cmd(0XF2);
    tft_write_data(0X00);

    tft_write_cmd(0X26);//Gamma设定
    tft_write_data(0X01);

    tft_write_cmd(0XE0);
    tft_write_data(0X0F);
    tft_write_data(0X22);
    tft_write_data(0X1C);
    tft_write_data(0X1B);
    tft_write_data(0X08);
    tft_write_data(0X0F);
    tft_write_data(0X48);
    tft_write_data(0XB8);
    tft_write_data(0X34);
    tft_write_data(0X05);
    tft_write_data(0X0C);
    tft_write_data(0X09);
    tft_write_data(0X0F);
    tft_write_data(0X07);
    tft_write_data(0X00);

    tft_write_cmd(0XE1);
    tft_write_data(0X00);
    tft_write_data(0X23);
    tft_write_data(0X24);
    tft_write_data(0X07);
    tft_write_data(0X10);
    tft_write_data(0X07);
    tft_write_data(0X38);
    tft_write_data(0X47);
    tft_write_data(0X4B);
    tft_write_data(0X0A);
    tft_write_data(0X13);//正常显示模式ON
    tft_write_data(0X06);
    tft_write_data(0X30);
    tft_write_data(0X38);
    tft_write_data(0X0F);
    tft_write_cmd(0X29);//显示开启
    //tft_write_cmd(0X28);//显示关闭

}



void tft_write_cmd(uint8_t cmd){
    DC_Command;
    tft_write_byte(cmd);
}

void tft_write_data(uint8_t data){
    DC_Data;
    tft_write_byte(data);
}

void tft_write_data_16bit(uint16_t data){
    tft_write_data(data>>8);
    tft_write_data(data);
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
               y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void tft_set_address(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
		tft_write_cmd(0x2a);//列地址设置
		tft_write_data_16bit(x1);
		tft_write_data_16bit(x2);
		tft_write_cmd(0x2b);//行地址设置
		tft_write_data_16bit(y1);
		tft_write_data_16bit(y2);
		tft_write_cmd(0x2c);//储存器写
}


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void tft_fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	tft_set_address(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(uint16_t i=ysta;i<yend;i++)
	{													   	 	
		for(uint16_t j=xsta;j<xend;j++)
		{
			tft_write_data_16bit(color);
		}
	} 					  	    
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void tft_point(uint16_t x,uint16_t y,uint16_t color)
{
	tft_set_address(x,y,x,y);//设置光标位置 
	tft_write_data_16bit(color);
} 