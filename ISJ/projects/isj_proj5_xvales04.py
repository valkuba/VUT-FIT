def gen_quiz(qpool, *qindex, altcodes="ABCDEF", quiz=None):

    if not quiz:
        quiz = []
    for idx in qindex:
        try:
            new = qpool[idx]
        except IndexError as e:
            print(f"Ignoring index {idx} - {e}")
        answer_list = []
        for item in  zip(altcodes, new[1]):
            answer = f"{item[0]}: {item[1]}"
            answer_list.append(answer)
        quiz_task = (new[0], answer_list)
        quiz.append(quiz_task)
    return quiz
