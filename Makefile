CXX=c++
CXXFLAGS=-Wall -Werror -Wextra -std=c++11
AR=ar
ARFLAGS=rcs
SOURCEFILES= \
			src/main.cpp
OBJECTS=$(SOURCEFILES:.cpp=.o)
NAME=libftpp.a
DEPS=$(OBJECTS:.o=.d)

all: $(OBJECTS) $(NAME)

$(NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 
	$(CXX) -MM $(CXXFLAGS) -MT $*.o $*.cpp > $*.d

clean:
	find . -name '*.o' -delete -print
	find . -name '*.d' -delete -print

fclean: clean
	rm -f $(NAME)

re: fclean all
