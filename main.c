#include <stm32f401xe.h>
#include <string.h>
#include <stdio.h>

void MSDelay(unsigned int);
void KeypadInit(void);
char KeyScan(void);
void lcdprint(char);
void lcdinit(void);
void sendcmd(char);
void printstr(char*);
void authenticate(void);
void ADCInit(void);
uint16_t ReadADC(void);
void DisplayADC(void);
void UARTInit(void);
void UART_Transmit(char*);

#define PASSWORD "1471"
char input_buffer[5]; // Stores user input
int input_index = 0;

void MSDelay(unsigned int Times) {
    unsigned int i, j;
    for (i = 0; i < Times; i++) {
        for (j = 0; j < 1400; j++);
    }
}

void UARTInit(void) {
    RCC->APB1ENR |= (1 << 17);  // Enable USART2 Clock
    RCC->AHB1ENR |= (1 << 0);   // Enable GPIOA Clock

    GPIOA->MODER |= (2 << 4) | (2 << 6);  // PA2 (TX) and PA3 (RX) as Alternate Function
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12); // Set Alternate Function 7 (USART2) for PA2, PA3

    USART2->BRR = 0x0683; // 9600 baud rate for 16 MHz clock
    USART2->CR1 |= (1 << 3) | (1 << 2); // Enable TX and RX
    USART2->CR1 |= (1 << 13); // Enable USART2
}

void UART_Transmit(char* str) {
    while (*str) {
        while (!(USART2->SR & (1 << 7))); // Wait for TX buffer empty
        USART2->DR = *str++;
    }
}

void DisplayADC(void) {
    char adc_val_str[15], voltage_str[18], step_size_str[15];
    uint16_t adc_val;
    float voltage;
    const float step_size = 5.0f / 4095.0f;

    while (1) {
        adc_val = ReadADC();
        voltage = adc_val * step_size;

        sprintf(adc_val_str, "ADC: %4d", adc_val);
        sprintf(voltage_str, "Voltage: %.4fV", voltage);
        sprintf(step_size_str, "Step: %.5fV", step_size);

        sendcmd(0x01); // Clear screen
        sendcmd(0x80); // Move cursor to first line
        printstr(adc_val_str);
        
        sendcmd(0xC0); // Move cursor to second line
        printstr(voltage_str);
        
        sendcmd(0x94); // Move cursor to third line
        printstr(step_size_str);
        
        // Send data to UART (Virtual Terminal)
        UART_Transmit("\r\n----------------------\r\n");
        UART_Transmit(adc_val_str);
        UART_Transmit("\r\n");
        UART_Transmit(voltage_str);
        UART_Transmit("\r\n");
        UART_Transmit(step_size_str);
        UART_Transmit("\r\n");

        MSDelay(500);
    }
}

void authenticate(void) {
    sendcmd(0x01); // Clear screen
    sendcmd(0x80); // Move cursor to first line

    if (strcmp(input_buffer, PASSWORD) == 0) {
        printstr("Access Granted!");
        UART_Transmit("\r\nAccess Granted! ADC Monitoring Started...\r\n");
        DisplayADC();
    } else {
        printstr("Access Denied!");
        UART_Transmit("\r\nAccess Denied!\r\n");
    }

    MSDelay(1000);
    sendcmd(0x01); // Clear screen again
    sendcmd(0x80);
    printstr("Enter Password:");
    sendcmd(0xC0); // Move to second line for input
    input_index = 0; // Reset input
}

void lcdprint(char ch) {
    GPIOA->ODR &= ~((0xFF << 5) | 0x03); // Clear PA5–PA12 + RS + EN
    GPIOA->ODR |= (ch << 5);             // Send data on PA5–PA12
    GPIOA->ODR |= 0x01;                  // RS = 1 (data)
    GPIOA->ODR |= 0x02;                  // EN = 1
    MSDelay(5);
    GPIOA->ODR &= ~0x02;                 // EN = 0
}



void KeypadInit(void) {
    GPIOB->MODER &= ~(0xFF003FFF);
    GPIOB->MODER |= (0x55000055);
    GPIOB->PUPDR &= ~(0X00003F00);
    GPIOB->PUPDR |= 0X00001500;
}

char KeyScan(void) {
    int temp;
    while (1) {
        GPIOB->ODR &= ~0X000000F;
        while ((GPIOB->IDR & 0X70) == 0X70);
        
        GPIOB->ODR &= ~0X00FF;
        GPIOB->ODR |= 0X0E;
        temp = GPIOB->IDR & 0X70;
        if (temp == 0X60) return '1';
        if (temp == 0X50) return '2';
        if (temp == 0X30) return '3';
        
        GPIOB->ODR &= ~0X00FF;
        GPIOB->ODR |= 0X0D;
        temp = GPIOB->IDR & 0X70;
        if (temp == 0X60) return '4';
        if (temp == 0X50) return '5';
        if (temp == 0X30) return '6';
        
        GPIOB->ODR &= ~0X00FF;
        GPIOB->ODR |= 0X0B;
        temp = GPIOB->IDR & 0X70;
        if (temp == 0X60) return '7';
        if (temp == 0X50) return '8';
        if (temp == 0X30) return '9';
        
        GPIOB->ODR &= ~0X00FF;
        GPIOB->ODR |= 0X07;
        temp = GPIOB->IDR & 0X70;
        if (temp == 0X60) return '*';
        if (temp == 0X50) return '0';
        if (temp == 0X30) return '#';
        
        MSDelay(200);
    }
}

uint16_t ReadADC(void) {
    ADC1->CR2 |= (1 << 30);  // Start conversion
    while (!(ADC1->SR & 2)); // Wait for conversion to complete
    return ADC1->DR;         // Return ADC data
}

void lcdinit(void) {
    // Configure PA5 to PA12 as output
    GPIOA->MODER &= ~(
        (3 << (5 * 2)) | (3 << (6 * 2)) | (3 << (7 * 2)) |
        (3 << (8 * 2)) | (3 << (9 * 2)) | (3 << (10 * 2)) |
        (3 << (11 * 2)) | (3 << (12 * 2))
    );
    GPIOA->MODER |= (
        (1 << (5 * 2)) | (1 << (6 * 2)) | (1 << (7 * 2)) |
        (1 << (8 * 2)) | (1 << (9 * 2)) | (1 << (10 * 2)) |
        (1 << (11 * 2)) | (1 << (12 * 2))
    );

    // Configure PA0 (RS) and PA1 (EN) as output
    GPIOA->MODER &= ~((3 << (0 * 2)) | (3 << (1 * 2)));
    GPIOA->MODER |= ((1 << (0 * 2)) | (1 << (1 * 2)));

    sendcmd(0x38); // 16x4 mode
    sendcmd(0x0F); // Display ON, Cursor blinking
    sendcmd(0x06); // Auto increment
    sendcmd(0x01); // Clear display
}

void sendcmd(char ch) {
    GPIOA->ODR &= ~((0xFF << 5) | 0x03); // Clear PA5–PA12 + RS + EN
    GPIOA->ODR |= (ch << 5);             // Send command on PA5–PA12
    GPIOA->ODR &= ~0x01;                 // RS = 0 (command)
    GPIOA->ODR |= 0x02;                  // EN = 1
    MSDelay(5);
    GPIOA->ODR &= ~0x02;                 // EN = 0
}


void printstr(char *str) {
    while (*str) {
        lcdprint(*str);
        str++;
    }
}

void ADCInit(void) {
    RCC->APB2ENR |= (1 << 8);  // Enable ADC1 clock
    GPIOA->MODER |= (3 << (4 * 2)); // PA4 in Analog Mode (0b11)
    GPIOA->PUPDR &= ~(3 << (4 * 2)); // No pull-up/down
    ADC1->CR1 = 0; // 12-bit resolution
    ADC1->CR2 = 0; // Disable ADC first
    ADC1->SQR3 = 4; // First conversion in regular sequence is PA4
    ADC1->CR2 |= (1 << 1); // Enable continuous conversion mode
    ADC1->CR2 |= 1; // Enable ADC
}

int main(void) {
    char key;

    RCC->AHB1ENR |= 3; // Enable GPIOA & GPIOB
   KeypadInit();
    lcdinit();
   ADCInit();
    UARTInit();

    sendcmd(0x80);
    printstr("Enter Password:");
    sendcmd(0xC0); // Move to second line

    while (1) {
        key = KeyScan();
        
        if (key == '*') {  // Enter key
            input_buffer[input_index] = '\0'; // Null terminate
            authenticate();
        } else if (key == '#') {  // Clear input
            input_index = 0;
            sendcmd(0x01);
            sendcmd(0x80);
            printstr("Enter Password:");
            sendcmd(0xC0);
        } else if (input_index < 4) {  // Capture input
            input_buffer[input_index++] = key;
            lcdprint('*'); // Mask input with ''
        }
        
        MSDelay(300);
    }
}
