# SDL2-Tetris-Game-INT2215 2024-UET

## Table Of Contents <br />
* [Hướng dẫn cài đặt](#setup)
* [Mô tả chung](#info)
* [Các chưng năng](#detail)
* [Kỹ thuật lập trình](#tech)
* [Các ý tưởng phát triển thêm](#future)
* [Kết luận](#summary)

* <a name="setup"/>

## 1. Hướng dẫn cài đặt <br />

- Sao chép repo trực tiếp
- Máy đã cài đặt các chuong trình SDL2, SDL Image, SDL Mixer, SDL ttf
- Đối với Code Block :
  - Liên kết Linker settings: -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
  - Vị trí thư mục include: Search directories | Compilers
  - Vị trí thư mục lib: Search directories | Linker
  - Dẫn đường link từng thư mục trên của SDL2, SDL Image, SDL Mixer, SDL ttf vào các đường dẫn như trên
  - Các bước trên cần thiết để cài thư viên SDL2 để có thể chạy được chương trình
- Nhấp đúp vào Tetris.cbp và F9 để chạy chương trình

* <a name="info"/>

## 2. Mô tả chung về trò chơi

### Giới thiệu

Tetris hay Xếp Hình (1984), trò chơi có 7 loại Khối Hình (Tetromino) I (thẳng đứng), J, L, O (vuông), S, T, Z ứng với 7 màu khác nhau.

### Về Game Mechanics <br />
- Cấu tạo: Mỗi Khối Hình được cấu tạo từ 4 khối gạch vuông (Block). Các Khối Hình có thể xoay được các góc 90, 180, 270 độ. <br />
- Mục tiêu: Di chuyển các khối gạch đang rơi từ từ xuống trong kích thước hình chữ nhật 20 hàng x 10 cột (trên màn hình).  <br />
- Chỗ nào có gạch rồi thì không di chuyển được tới vị trí đó. <br />
- Người chơi xếp những khối hình sao cho khối hình lấp đầy 1 hàng ngang để ghi điểm và hàng ngang ấy sẽ biến mất và người chơi sẽ ghi điểm. <br />
- Người chơi thua cuộc khi Khối Hình tràn ra trên 1 dòng khỏi Grid (Lưới). <br />

* <a name="detail"/>
## 3. Các chức năng
- Để thoát game, có thể ấn nút X ở góc trên bên phải của màn hình.

### Chức năng Play <br />
- Tạo các block với màu sắc ngẫu nhiên
- Xử lý các block khi va chạm với nhau, va chạm với tường và với đáy
- Di chuyển và xoay các block 90 độ, 180 độ, 270 độ
- Xóa hàng khi các block lấp đầy hàng ngang
- Tính điểm
- Hiện màn hình cho biết thông số điểm của người chơi và điểm cao nhất từng đạt được
- Tạm dừng màn chơi khi cần thiết
- Âm thanh nền và âm thanh hiệu ứng

### Chức năng Help <br />
- Chỉ dẫn người chơi cách các nút hoạt động trong game

### Chức năng Setting <br />
- Điều chỉnh phần tiếng dộng của game 

<a name="tech"/>

### 4. Kỹ thuật lập trình

- Mảng
- Class, struct
- Con trỏ
- Đồ họa (Hình ảnh, âm thanh, text, sự kiện chuột và bàn phím)
- Tách file
- Cấu trúc rẽ nhánh
- Vòng lặp

<a name="future"/>

### 5. Các ý tưởng phát triển thêm

- Phát triển thêm chế độ mutiplayer
- Phát triển level khó và nhanh hơn
- Tối ưu code

<a name="summary">
  
### 6. Kết luận
- Em đã cố gắng để có thể hoàn thành game tốt nhất có thể tuy nhiên code thật sự còn nhiều thiếu sót
A Special Thanks To:<br />
- Cô Nguyễn Thị Hải Yến
- Cô Trần Thị Minh Châu
- Và các bạn lớp INT2215 2024 1.<br />

Đã giúp em fix bug game trong suốt quá trình học
## Em xin trân trọng cảm ơn thầy cô và các bạn đã dành thời gian lắng nghe. <br />

  
