#Ускорение
def sn(lst1,lst2):
	new_lst = []
	for index in range(0,6):
		new_lst.append(lst1[index]/lst2[index])
	return new_lst

#Эффективность
def effect(lst, p):
	new_lst=[]
	for index in range(0,6):
		new_lst.append(lst[index]/p)
	return new_lst

#Стоимость
def count(lst, p):
	new_lst = []
	for index in range(0,6):
		new_lst.append(lst[index] * p)
	return new_lst

t1 = [0.671, 1.08, 1.902, 2.955, 5.624, 10.789]
t2 = [0.605, 0.665, 1.114, 1.89, 3.962, 6.424]
t4 = [0.468, 0.581, 1.342, 2.075, 3.201, 4,808]
t8 = [0.368, 0.581, 1.346, 2.079, 3.282, 4.764]
t16 = [0.351, 0.559, 1.221, 2.039, 3.181, 4.662]
t32 = [0.417, 0.711, 1.354, 2.183, 3.271, 4.834]


print(sn(t1, t32))
print(effect(sn(t1, t32),32))
print(count(t32, 32))