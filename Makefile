CXX			= c++ -std=c++17

NAME		= marvin

RM			= rm -rf

SRCS_DIR	= src

OBJS_DIR	= obj

SRCS   	= \
		$(SRCS_DIR)/main.cpp

OBJS		= $(SRCS:${SRCS_DIR}/%.cpp=${OBJS_DIR}/%.o)

DEPS	= $(OBJS:%.o=%.d)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)  -lpthread

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all		: $(NAME)

fclean	: clean

clean:
	$(RM) $(OBJS)

re		: fclean all

-include $(DEPS)

.PHONY	: all clean fclean re