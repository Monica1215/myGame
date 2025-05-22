
## Đinh Thị Hậu 24021462
# Chủ đề game
Shapes Escape - di chuyển nhân vật né các vật cản hình khối
Ý tưởng lấy cảm hứng từ: Just Shapes and Beats
## Instruction:
Dùng phím mũi tên để di chuyển.
Space để lướt (chỉ xuất hiện sau 30s chơi)
Trong khi ở game phase nhấn quit để pause game.
# Checklist
1. Lấy code mẫu:
- graphics.h, music.h, font.h: code mẫu từ sites hướng dẫn SDL
- texture : lazyfoo
Những phần còn lại là tự làm.
2. Credit:
Nhạc: popstar love - In Love With A Ghost
Hình ảnh: flaticon.com
SFX: Freesound.org
3. Lệnh vẽ hình
4. Texture
5. Background: Line hiện, background tĩnh
6. Event bàn phím
7. Event chuột
8. Animation: chuyển động của nhân vật để lại dấu, hình khối có hiệu ứng
9. Xử lý va chạm
10. Score: có score và high score
11. Lưu bảng điểm 
12. 13. SOund và Sound on/off
14. Background music
15. Font: 2 font
16. Menu: menu pause, menu intro, menu game over
17. Pause
18. Lives

##Ngưỡng điểm mong muốn 9-10:
Game có nhiều hiệu ứng của đạn, hệ thống đạn đa dạng, và nhân vật.
Nhân vật:
- Di chuyển mượt, không tràn màn hình, lướt(dash) được.
- Có đuôi, vệt sáng.
- Có hiệu ứng nhấp nháy và thay đổi kích thước khi mất mạng
Đạn: - có hiệu ứng cảnh báo, làm mờ, nhấp nháy nhẹ
Game play:
- Có tính điểm theo thời gian sống sót, có lưu highscore
- 4 phase lặp lại vô hạn, thời gian cooldown của đạn giảm dần
