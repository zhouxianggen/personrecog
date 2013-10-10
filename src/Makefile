VPATH=./crf
crfs = encoder.o feature.o lbfgs.o node.o path.o feature_cache.o feature_index.o libcrfpp.o param.o tagger.o 

run_test : test.o config.o personrecog.o $(crfs)
	g++ -o $@ $^  -lpthread
	rm -rf *.o

run_server : server.o mongoose.o config.o personrecog.o $(crfs)
	g++ -o $@ $^  -lpthread -ldl
	rm -rf *.o

clean :
	rm -rf run_*
