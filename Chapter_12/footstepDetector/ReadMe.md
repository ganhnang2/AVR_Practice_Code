# Project: Phát hiện bước chân (Footstep Detector)

Ứng dụng thực tế của tôi cho bộ dò rung piezo chính là một hệ thống đèn cầu thang kích hoạt bằng bước chân trong nhà 

## Nguyên lý hoạt động
Thiết bị của tôi bật dải đèn trong vài giây khi phát hiện rung động do bước chân trên cầu thang, và tự động tắt khi cầu thang yên tĩnh trở lại </p>
-> Vì tôi không biết bướcc chân củaa mình sẽ mạnh đến mức nào nên muốn có một biến có khả năng tự hiệu chỉnh phần nào 

## Các bước thực hiện dự án
1. Đầu tiên, phải xác định được chính xác điện áp của mạch phân cực với R = 1Mohm </p>
-> Giải pháp: Lấy trung bình trên một khoảng dài và dùng giá trị đó làm điện áp phân cực. </p>
-> Sử dụng phương pháp trung bình trượt có trọng số theo hàm mũ ( (EWMA)
2. Lựa chọn một giá trị ở 2 phía của giá trị trung tâm và quy định rằng khi ADC đọc vượt những giá trị này thì phát hiện bước chân
3. Mẹo tiếp theo là sử dụng moving average để đặt ngưỡng phát hiễn bước chân </p>
-> Vơi mỗi giá trị high, low và middle thì ta sử dụng công thức được tối ưu hóa (trình bày bên dưới) để làm "mượt" dữ liệu.

## Trình bày về phương pháp EWMA
### Giới thiệu

G/sử: Cảm biến đọc giá trị mỗi giây 
-  Giá trị đo được gọi là xt </p>
-> List các dữ liệu đo được trong khoảng thời gian: x0, x1, ..., xn
-  Trung bình của chuỗi trên gọi là yt. Mỗi khi có một giá trị cảm biến mới xt thì cập nhật yt </p>
-> Khi này, tạo ra mỗi chuỗi y0, y1, ..., yn

### Cách thức hoạt động

Lấy trung bình của một tập dữ liệu tại một thời điểm </p>
-> Khi sensor đọc được dữ liệu mới thì loại bỏ dữ liệu cũ nhất và tính toán lại giá trị trung bình </p>
-> Giá trị  trung bình sẽ dịch chuyển theo tập dữ liệu - "trượt" của moving average

Chúng ta chỉ cần tính trung bình 2 giá trị: 
- Giá trị hiện tại (xt) 
- Giá trị trung bình từ lần quan sát trước (yt-1) </p>

-> Ta sẽ lập công thức tính được giá trị trung bình hiện tại (yt): với trọng số cho 2 giá trị này là khác nhau (Nhìn biểu thức dưới là biết)


### Thiết lập công thức 

Công thức ban đầu:

$$
y_t = \frac{1}{16}x_t + \frac{15}{16}y_{t-1}
$$


</p>

Công thức đượcc tối ưu hóa với sử dụng khi lập trình C (tránh làm tròn xuống):

$$
16y_t = x_t + 16y_{t-1} - \frac{16y_{t-1} - 16/2}{16}
$$
