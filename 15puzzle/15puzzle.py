# coding: utf-8
# おまけ...普通の15puzzleをpythonで↓
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

	wnum = 0
	key = 15
	while (wnum < num):
		move_viability = []
		if key < 12:
			move_viability.append(4)
		if key > 3:
			move_viability.append(-4)
		if key % 4 != 3:
			move_viability.append(1)
		if key % 4 != 0:
			move_viability.append(-1)

		change = random.choice(move_viability)

		tmp_piece = piece[key]
		piece[key] = piece[key + change]
		piece[key + change] = tmp_piece

		key = key + change

		wnum = wnum + 1
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
	complete_pazzle = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0]

	shuffle_pazzle = list(complete_pazzle)
	shuffle_pazzle, key = shuffle(shuffle_pazzle, 1)

	bleak_check = False
	while complete(shuffle_pazzle, complete_pazzle):
		check_tmp = True
		direction = ""

		while check_tmp:
			draw(shuffle_pazzle)

			direction = raw_input("up:w, right:d, down:s, left:a, retire:0\n")
			check_tmp = input_check(direction, key)
			if direction == "0":
				bleak_check = True
				break
			if check_tmp:
				print "error"
		if bleak_check:
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

		print str(change) + " " + str(key)

		tmp_piece = shuffle_pazzle[key]
		shuffle_pazzle[key] = shuffle_pazzle[key + change]
		shuffle_pazzle[key + change] = tmp_piece

		key = key + change

	if bleak_check != True:
		draw(shuffle_pazzle)
		print "*Clear*"
