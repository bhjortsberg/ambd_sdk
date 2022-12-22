cp project_lp/asdk/image/km0_boot_all.bin ../../../tools/AmebaD/image_tool_cli/
cp project_hp/asdk/image/km4_boot_all.bin ../../../tools/AmebaD/image_tool_cli/
cp project_lp/asdk/image/km0_km4_image2.bin ../../../tools/AmebaD/image_tool_cli/
read -p "Set Ameba D device in UART download mode then press a enter..."
cd ../../../tools/AmebaD/image_tool_cli/
./amebad_image_tool.exe COM4
cd -
