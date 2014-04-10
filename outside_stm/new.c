
//   nrf24l01base_initialize_debug(true, 1, false);  // setup wifibase as a receiver, Standard Shockburst
nrf24l01base_initialize_debug(true, 32, true);       // Enhanced Shockburst, Auto Ack turned on
nrf24l01base_clear_flush();
printf("Begin wifi test\n");
// Base Reception
char rxdata[32];
char index;
while(!(nrf24l01base_irq_pin_active() && nrf24l01base_irq_rx_dr_active()));
nrf24l01base_read_rx_payload(rxdata, 32);
nrf24l01base_irq_clear_all();
f3d_delay_uS(130);

// Payload Translation
printf("Base Data: ");
for (index=0;index<32;index++) {
				printf("%c",rxdata[index]);
}
printf("\n");

for (index=0;index<32;index++) {
				if ((rxdata[index] >= 'a') && (rxdata[index] <= 'z')) {
								rxdata[index]-=32;
				}
}

printf("Base: receive character: %c\n",rxdata);
// rxdata-=32; 
printf("Base: upcase: %c\n",rxdata);

