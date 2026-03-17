# Kỹ thuật Oversampling

Kỹ thuật Oversampling (Lấy mẫu quá mức) là một phương pháp quan trọng được áp dụng trong hai lĩnh vực chính: 
1. Xử lý tín hiệu số (DSP/Embedded Systems)
2. Khoa học dữ liệu (Data Science).

Trong bài này, ta sẽ bàn luận về việc sử dụng kỹ thuật này tăng độ phân giải cho ADC </p>

## Oversampling trong Xử lý Tín hiệu & ADC (Embedded Systems)
Trong các vi điều khiển như AVR, bộ ADC thường có độ phân giải cố định (ví dụ 10-bit). Oversampling giúp bạn đạt được độ phân giải cao hơn (12-bit, 14-bit...) và giảm nhiễu mà không cần mua chip ADC đắt tiền hơn.

Quy tắc vàng: Để tăng thêm n bit độ phân giải, bạn cần lấy 4^(n) mẫu. </p>
**Các bước áp dụng thực tế (Ví dụ trên AVR):**

Giả sử bạn muốn tăng độ phân giải từ 10-bit lên 12-bit (tăng thêm n = 2 bit)

*Bước 1*: Tính số lượng mẫu cần thiết áp dụng công thức 4^(n):
> Sample = 4^(2) = 16

*Bước 2*: Cộng dồn (Accumulation) Trong code, bạn tạo một biến lưu trữ lớn hơn (ví dụ uint16_t hoặc uint32_t) để chứa tổng của 16 lần đọc này

> - Đọc ADC lần 1: giá trị 512
> - ...
> - Đọc ADC lần 16: giá trị 515
> - Tổng: Sum = 8200 (Ví dụ)

*Bước 3*: Dịch bit (Decimation/Right Shift) Để lấy giá trị trung bình đã được tăng cường độ phân giải, bạn không chia cho 16 (chia thường). Thay vào đó, bạn dịch phải (bit-shift) số lượng bit tương ứng.
Công thức dịch bit là: Dịch phải n bit. Ở đây n = 2, vậy ta dịch phải 2 bit (tương đương chia cho 2^(2) = 4, chứ không phải chia cho tổng số mẫu là 16).

> Kết quả (12 bit) = Sum >> 2

### Giải thích tại sao lại dịch phải n bit 
Khi bạn cộng dồn 16 mẫu 10-bit, bạn sẽ tạo ra một biến số lớn hơn, có thể chứa tối đa 14-bit
thông tin. Hãy hình dung biên Sum 14-bit này được cấu tạo từ 3 phần:

>	AAAA AAAA AA	BB		CC

- Phần A (10 bit đầu): Là phần tín hiệu mạnh nhất, tương đương với giá trị ADC gốc.
- Phần B (2 bit giữa): Đây là phần "vàng". Nhờ cộng dồn nhiễu, thông tin chi tiết (phần lẻ) đã dồn vào đây. Đây chính là thứ giúp ta có 12-bit.
- Phần C (2 bit cuối): Đây là nhiễu ngẫu nhiên quá nhỏ, không còn giá trị thống kê chính xác.

**So sánh 2 hành động**

1. Hành động 1: Chia 16 (Tương đương dịch phải 4 bit - >> 4)

Bạn muốn quay về 10-bit để khử nhiễu. Bạn dịch 4 bit, nghĩa là bạn gạt bỏ 4 bit cuối:
- Chuỗi ban đầu: AAAA AAAA AA BB CC
- Kết quả còn lại: 0000 AAAA AAAA AA
- Cả BB và CC đều bị vứt đi. Bạn mất sạch thông tin chi tiết

2. Hành động 2: Chia 4 (Tương đương dịch phải 2 bit - >> 2)

Đây là kỹ thuật Oversampling 12-bit. Bạn chỉ dịch 2 bit:
- Chuỗi ban đầu: AAAA AAAA AA BB CC
- Kết quả còn lại: 00 AAAA AAAA AA BB
- Chỉ có CC (rác) bị vứt đi.
- Phần BB được giữ lại và trở thành 2 bit cuối cùng (LSB) của số 12-bit mới.

## Ví dụ minh họa
Hãy lấy ví dụ giá trị 512.5 (trên thang 10-bit)
- Thang 12-bit (gấp 4 lần): Giá trị lý tưởng là 512.5 x 4 = 2050
- Tổng 16 mẫu (Sum) sẽ xấp xỉ: 512.5 x 16 = 8200 (Giả sử)

Hãy xem số 8200 trong hệ nhị phân (14-bit): 10 0000 0000 10 00

Phân tích chuỗi này theo cấu trúc A-B-C ở trên:
- Phần A (Gốc): 10 0000 0000 (Là số 512)
- Phần B (Thông tin thêm): 10 (Số 2 nhị phân, đại diện cho phần 0.5)
- Phần C (Rác): 00

**Bây giờ thực hiện lệnh Sum >> 2:**
- Máy tính đẩy toàn bộ dãy bit sang phải 2 bước.
- Hai bit cuối cùng (00 - Phần C) rơi xuống vực và biến mất.
- Phần B (10) trượt xuống vị trí cuối cùng của chuỗi số mới.

**Kết quả thu được (12-bit): 10 0000 0000 10 (Đây chính là số 2050 trên thang 12-bit)**

_Tóm lại: Việc dịch ít hơn thực chất là phóng to thang đo. Thay vì cố gắng biểu diễn số thập phân (512.5)
mà vi điều khiển không hiểu, ta biểu diễn nó dưới dạng một số nguyên lớn hơn (2050) ở một thang đo chi tiết hơn._