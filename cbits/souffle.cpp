#include "souffle/SouffleInterface.h"
#include "souffle.h"


extern "C" {
    souffle_t* souffle_init(const char* progName) {
        auto prog = souffle::ProgramFactory::newInstance(progName);
        return reinterpret_cast<souffle_t*>(prog);
    }

    void souffle_free(souffle_t* program) {
        auto prog = reinterpret_cast<souffle::SouffleProgram*>(program);
        assert(prog);
        delete prog;
    }

    void souffle_run(souffle_t* program) {
        auto prog = reinterpret_cast<souffle::SouffleProgram*>(program);
        assert(prog);
        prog->run();
    }

    void souffle_load_all(souffle_t* program, const char* directory) {
        auto prog = reinterpret_cast<souffle::SouffleProgram*>(program);
        assert(prog);
        assert(directory);
        prog->loadAll(directory);
    }

    void souffle_print_all(souffle_t* program) {
        auto prog = reinterpret_cast<souffle::SouffleProgram*>(program);
        assert(prog);
        prog->printAll();
    }

    relation_t* souffle_relation(souffle_t* program, const char* relation_name) {
        auto prog = reinterpret_cast<souffle::SouffleProgram*>(program);
        assert(prog);
        assert(relation_name);
        auto relation = prog->getRelation(relation_name);
        assert(relation);
        return reinterpret_cast<relation_t*>(relation);
    }

    struct relation_iterator {
        using iterator_t = souffle::Relation::iterator;
        iterator_t iterator;
        iterator_t end;

        relation_iterator(const iterator_t& begin_, const iterator_t& end_)
            : iterator(begin_)
            , end(end_) {}
    };

    relation_iterator_t* souffle_relation_iterator(relation_t* relation) {
        auto rel = reinterpret_cast<souffle::Relation*>(relation);
        assert(rel);
        relation_iterator_t* it = new relation_iterator_t(rel->begin(), rel->end());
        return it;
    }

    void souffle_relation_iterator_free(relation_iterator_t* iterator) {
        assert(iterator);
        delete iterator;
    }

    bool souffle_relation_iterator_has_next(const relation_iterator_t* iterator) {
        assert(iterator);
        return iterator->iterator != iterator->end;
    }

    tuple_t* souffle_relation_iterator_next(relation_iterator_t* iterator) {
        assert(iterator);
        auto tuple = reinterpret_cast<tuple_t*>(&*iterator->iterator);
        ++iterator->iterator;
        return tuple;
    }

    tuple_t *souffle_tuple_alloc(relation_t* relation) {
        auto rel = reinterpret_cast<souffle::Relation*>(relation);
        auto tuple = new souffle::tuple(rel);
        return reinterpret_cast<tuple_t*>(tuple);
    }

    void souffle_tuple_free(tuple_t* tuple) {
        auto t = reinterpret_cast<souffle::tuple*>(tuple);
        assert(t);
        delete t;
    }

    void souffle_tuple_push_int(tuple_t* tuple, int32_t value) {
        auto t = reinterpret_cast<souffle::tuple*>(tuple);
        assert(t);
        *t << value;
    }

    void souffle_tuple_push_string(tuple_t* tuple, const char* value) {
        auto t = reinterpret_cast<souffle::tuple*>(tuple);
        assert(t);
        *t << value;
    }

    void souffle_tuple_add(relation_t* relation, tuple_t* tuple) {
        auto rel = reinterpret_cast<souffle::Relation*>(relation);
        auto t = reinterpret_cast<souffle::tuple*>(tuple);
        assert(rel);
        assert(t);
        rel->insert(*t);
    }

    void souffle_tuple_pop_int(tuple_t* tuple, int32_t* result) {
        auto t = reinterpret_cast<souffle::tuple*>(tuple);
        assert(t);
        assert(result);
        *t >> *result;
    }

    void souffle_tuple_pop_string(tuple_t* tuple, char** result) {
        auto t = reinterpret_cast<souffle::tuple*>(tuple);
        assert(t);
        assert(result);
        std::string value;
        *t >> value;
        *result = strdup(value.c_str());
    }
}
