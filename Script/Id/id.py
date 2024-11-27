import tkinter as tk
from ttkbootstrap import Style
import sqlite3

# 身份证校验函数
def validate_id_card(id_card):
    if len(id_card) != 18:
        return False
    weight_factors = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2]
    checksum = sum(int(id_card[i]) * weight_factors[i] for i in range(17))
    checksum %= 11
    check_digit = ['1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2']
    expected_check_digit = check_digit[checksum]
    actual_check_digit = id_card[-1].upper() if id_card[-1].isdigit() else 'X'
    return expected_check_digit == actual_check_digit

# 获取身份证信息函数
def get_id_info(id_card):
    if not validate_id_card(id_card):
        return "身份证号码不合法", None, None

    birth_date = id_card[6:14]
    gender_code = int(id_card[16])
    gender = '男' if gender_code % 2 else '女'


    #省
    province = id_card[:2]+'0000'
    province_address = query_address(province)
    #市
    city = id_card[:4]+'00'
    city_address = query_address(city)
    #县
    county = id_card[:6]
    county_address =query_address(county)
    #完整地址
    address = province_address+'-'+city_address+'-'+county_address


    return birth_date, gender, address

# 查询地址信息函数
def query_address(id_code):
    try:
        conn = sqlite3.connect('id_cards.db')
        cursor = conn.cursor()
        cursor.execute("SELECT address FROM id_card WHERE id_code=?", (id_code,))
        result = cursor.fetchone()
        conn.close()
        return result[0] if result else "地址信息未找到"
    except sqlite3.Error as e:
        print(f"数据库查询失败: {e}")
        return "数据库查询失败"

# 更新结果显示区域
def update_result_area(id_card):
    result_text.delete('1.0', tk.END)
    info = get_id_info(id_card)
    if info[0]:
        result_text.insert(tk.END, f"身份证号码：{id_card}\n")
        result_text.insert(tk.END, f"性别：{info[1]}\n")
        result_text.insert(tk.END, f"生日：{info[0]}\n")
        result_text.insert(tk.END, f"籍贯：{info[2]}\n")
    else:
        result_text.insert(tk.END, info[0] + "\n")

# 主窗口
root = tk.Tk()
root.title("身份证校验器")

# 样式
style = Style(theme='litera')

# 输入区域
input_frame = tk.Frame(root)
input_frame.pack(pady=10)
id_card_entry = tk.Entry(input_frame, width=30, borderwidth=2)
id_card_entry.pack(side=tk.LEFT, padx=(5, 10))

# 按钮区域
button_frame = tk.Frame(root)
button_frame.pack(pady=10)
validate_button = tk.Button(button_frame, text="验证", command=lambda: update_result_area(id_card_entry.get()))
validate_button.pack(side=tk.LEFT, padx=(5, 10))

# 结果显示区域
result_text = tk.Text(root, height=10, width=50, borderwidth=2)
result_text.pack(pady=10)

# 运行主循环
root.mainloop()