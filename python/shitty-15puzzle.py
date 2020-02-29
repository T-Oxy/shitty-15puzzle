# coding: utf-8
import random

def draw(pzl):
    count = 0
    draw_text = ""
    for piece in pzl:
        draw_text = draw_text + str(piece).zfill(2) + " "
        count = count + 1
        
        if count == 4:
            print draw_text
            draw_text = ""
            count = 0

def shuffle(pzl, num):
    piece = pzl
#クソゲー化...解答不可能な配置に変更
    piece[13] = 15
    piece[14] = 14
    
    t = 0
    key = 15
    while (t < num):
        viability = []    #移動可能性リスト
        if key < 12:    #最下でない場合
            viability.append(4)
        if key > 3:    #最上でない場合
            viability.append(-4)
        if key % 4 != 3:    #最右でない場合
            viability.append(1)
        if key % 4 != 0:    #最左でない場合
            viability.append(-1)
        change = random.choice(viability)   #シャッフル対象パネルを選択
        
        tmp_piece = piece[key]
        piece[key] = piece[key + change]
        piece[key + change] = tmp_piece
    
        key = key + change
        t = t + 1
    return (piece, key)

def complete(check, correct):
    conti = True
    if check == correct:
        conti = False
    return conti

def input_check(dir, key):
    check = True
    if (dir == "w" and key > 3) or (dir == "d" and key % 4 != 3) or (dir == "s" and key < 12) or (dir == "a" and key % 4 != 0):
        check = False
    return check

if __name__ == '__main__':
    complete_puzzle = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0]   #完成確認用リスト
    puzzle = list(complete_puzzle)     #操作用リスト
    puzzle, key = shuffle(puzzle, 100)    #シャッフル(100回)
    
    b_check = False
    while complete(puzzle, complete_puzzle):   #完成チェック
        check_tmp = True
        direction = ""
        
        while check_tmp:
            draw(puzzle)
            direction = raw_input("UP:w, RIGHT:d, DOWN:s, LEFT:a, QUIT:q\n")
            check_tmp = input_check(direction, key)
            if direction == "q":
                b_check = True
                break
            if check_tmp:
                print "E R R O R..."
        if b_check:
            break
        
        change = 0
        if direction == "w":
            change = -4
        elif direction == "d":
            change = 1
        elif direction == "s":
            change = 4
        elif direction == "a":
            change = -1

        tmp_piece = puzzle[key]
        puzzle[key] = puzzle[key + change]
        puzzle[key + change] = tmp_piece
        key = key + change

    if b_check != True:
        draw(puzzle)
        print "C L E A R !!"
