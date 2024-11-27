import binascii

def base64_to_hex(input_str):
    decoded_bytes = binascii.a2b_base64(input_str)
    hex_str = binascii.hexlify(decoded_bytes).decode('utf-8').upper()

    # 添加固定格式行
    format_line = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"
    separator = "-" * 49
    formatted_result = f"{format_line}\n{separator}\n"

    # 分割结果成每行16个数
    for i in range(0, len(hex_str), 32):
        formatted_result += ' '.join(hex_str[i:i+2] for i in range(i, i+32, 2)) + "\n"

    return formatted_result

def hex_to_base64(hex_str):
    decoded_bytes = binascii.unhexlify(hex_str)
    base64_str = binascii.b2a_base64(decoded_bytes).decode('utf-8').strip()
    return base64_str

def main():
    while True:
        print("选择要进行的操作:")
        print("1. Base64转换为十六进制")
        print("2. 十六进制转换为Base64")
        print("3. 退出")
        choice = input("请选择操作（1/2/3）：")

        if choice == "1":
            input_str = input("请输入要转换的Base64内容：")
            result_hex = base64_to_hex(input_str)
            print("转换结果为十六进制：")
            print(result_hex)
        elif choice == "2":
            input_str = input("请输入要转换的十六进制内容：")
            result_base64 = hex_to_base64(input_str)
            print("转换结果为Base64：" + result_base64)
        elif choice == "3":
            print("退出程序。")
            break
        else:
            print("无效的选择，请重新输入。")

if __name__ == "__main__":
    main()
