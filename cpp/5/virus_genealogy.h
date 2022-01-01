ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include <vector>
#include <map>
#include <exception>
#include <memory>

struct VirusNotFound : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "VirusNotFound";
    }
};

struct VirusAlreadyCreated : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "VirusAlreadyCreated";
    }
};

struct TriedToRemoveStemVirus : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "TriedToRemoveStemVirus";
    }
};

template<typename Virus>
class VirusGenealogy {
    class ListElement {
        std::shared_ptr<Virus> value;
        std::weak_ptr<ListElement> left;
        std::shared_ptr<ListElement> right;
    public:
        explicit ListElement() : value(nullptr) {};

        explicit ListElement(std::shared_ptr<Virus> value) : value(value) {
            left = std::make_shared<ListElement>();
        };

        void add_left(std::weak_ptr<ListElement> l) {
            left.reset();
            left = l;
        }

        void add_right(std::shared_ptr<ListElement> r) {
            right.reset();
            right = r;
        }

        Virus const &get_value() {
            return *value;
        }

        std::weak_ptr<ListElement> get_left() {
            return left;
        }

        std::shared_ptr<ListElement> get_right() {
            return right;
        }

        void remove() {
            auto left_lock = left.lock();
            if (left_lock) {
                left_lock->add_right(right);
            }
            if (right) {
                right->add_left(left);
            }
        }
    };

public:
    class children_iterator : public std::iterator<std::bidirectional_iterator_tag, Virus> {
        std::shared_ptr<ListElement> element;
    public:
        explicit children_iterator() = default;

        explicit children_iterator(std::shared_ptr<ListElement> head) : element(head) {}

        Virus const &operator*() const {
            return element->get_value();
        }

        std::shared_ptr<Virus> operator->() const {
            return std::make_shared<Virus>(element->get_value());
        }

        children_iterator &operator++() {
            element = element->get_right();
            return *this;
        }

        children_iterator operator++(int) {
            children_iterator result = *this;
            element = element->get_right();
            return result;
        }

        children_iterator &operator--() {
            element = element->get_left();
            return *this;
        }

        children_iterator operator--(int) {
            children_iterator result = *this;
            (*this)--;
            return result;
        }

        friend bool operator==(children_iterator a, children_iterator b) {
            return &(*a.element) == &(*b.element);
        }

        friend bool operator!=(children_iterator a, children_iterator b) {
            return !(a == b);
        }
    };

private:
    class List {
        std::shared_ptr<ListElement> head;
        std::shared_ptr<ListElement> tail;
    public:
        List() {
            tail = std::make_shared<ListElement>();
            head = tail;
        }

        void add(std::shared_ptr<Virus> value) {
            auto new_list_element = std::make_shared<ListElement>(value);
            head->add_left(new_list_element);
            auto left = head->get_left();
            auto left_locked = left.lock();
            left_locked->add_right(head);
            head = (head->get_left()).lock();
        }

        std::shared_ptr<ListElement> find(std::shared_ptr<Virus> value) {
            std::shared_ptr<ListElement> element = head;
            while (element) {
                if (&(element->get_value()) == &(*value)) {
                    return element;
                }
                element = element->get_right();
            }
            return nullptr;
        }

        void remove(std::shared_ptr<Virus> value) {
            std::shared_ptr<ListElement> found = find(value);
            if (found) {
                found->remove();
            }
        }

        children_iterator get_begin() const {
            return children_iterator(head);
        }

        children_iterator get_end() const {
            return children_iterator(tail);
        }
    };

    class Node {
        using parents_list_t = std::vector<typename Virus::id_type>;
        using children_nodes_list_t = std::vector<std::shared_ptr<Node>>;
        using parent_nodes_list_t = std::vector<std::weak_ptr<Node>>;

        typename Virus::id_type id;
        std::shared_ptr<Virus> virus;
        List children;
        parents_list_t parents;
        parents_list_t parents_copy;
        children_nodes_list_t children_nodes;
        parent_nodes_list_t parent_nodes;
        parent_nodes_list_t parent_nodes_copy;
    public:
        explicit Node(typename Virus::id_type const &id) : id(id) {
            virus = std::make_shared<Virus>(id);
            children = List();
        }

        void add_parent(typename Virus::id_type parent_id, std::shared_ptr<Node> parent_node) {
            for (auto it = parents.begin(); it != parents.end(); it++) {
                if (*it == parent_id) {
                    return;
                }
            }
            try {
                parents_copy.push_back(parent_id);
                parent_nodes_copy.push_back(parent_node);
            }
            catch (...) {
                parents_copy = parents;
                parent_nodes_copy = parent_nodes;
                throw;
            }
            parents = parents_copy;
            parent_nodes = parent_nodes_copy;
        }

        void add_child(std::shared_ptr<Virus> child, std::shared_ptr<Node> child_node) {
            if (children.find(child)) {
                return;
            }

            children.add(child);
            children_nodes.push_back(child_node);
        }

        parents_list_t remove_parent(typename Virus::id_type& parent_id) {
            for (auto it = parents_copy.begin(); it != parents_copy.end(); it++) {
                if (*it == parent_id) {
                    parents_copy.erase(it);
                    break;
                }
            }

            try {
                for (auto it = parent_nodes_copy.begin(); it != parent_nodes_copy.end(); it++) {
                    if (((*it).lock())->id == parent_id) {
                        parent_nodes_copy.erase(it);
                        break;
                    }
                }

                if (parents_copy.empty()) {
                    auto result = destroy();
                    result.emplace_back(id);
                    return result;
                }

                parents = parents_copy;
                parent_nodes = parent_nodes_copy;
                return {};
            }
            catch (...) {
                parents_copy = parents;
                parent_nodes_copy = parent_nodes;
                throw;
            }
        }

        void remove_child(std::shared_ptr<Virus> child) {
            children.remove(child);
            for (auto it = children_nodes.begin(); it != children_nodes.end(); it++) {
                if (((*it)->get_virus()) == child) {
                    children_nodes.erase(it);
                    break;
                }
            }
        }

        typename VirusGenealogy<Virus>::children_iterator get_children_begin() const {
            return children.get_begin();
        }

        typename VirusGenealogy<Virus>::children_iterator get_children_end() const {
            return children.get_end();
        }

        parents_list_t get_parents() const {
            return parents;
        }

        std::shared_ptr<Virus> get_virus() {
            return virus;
        }

        parents_list_t destroy() {
            parents_copy.clear();
            parents_list_t deleted;
            for (auto& child: children_nodes) {
                parents_list_t del_from_child = child->remove_parent(id);
                deleted.insert(deleted.end(), del_from_child.begin(), del_from_child.end());
            }

            return deleted;
        }

        void commit_destroy(bool first) {
            if (parents_copy.empty() || first) {
                for (auto& parent: parent_nodes_copy) {
                    auto parent_locked = parent.lock();
                    parent_locked->remove_child(get_virus());
                }

                for (auto& child: children_nodes) {
                    child->commit_destroy(false);
                }
            }
            parents = parents_copy;
            parent_nodes = parent_nodes_copy;
        }

        void revert_destroy() {
            parents_copy = parents;
            parent_nodes_copy = parent_nodes;

            for (auto& child: children_nodes) {
                child->revert_destroy();
            }
        }
    };

    using virus_map_t = std::map<typename Virus::id_type, std::shared_ptr<Node>>;
    typename Virus::id_type stem_id;
    virus_map_t virus_map;

    auto check_if_virus_present(typename Virus::id_type const &id) const {
        auto it = virus_map.find(id);

        if (it == virus_map.end()) {
            throw VirusNotFound();
        }
        return it;
    }

public:
    // Tworzy nową genealogię.
    // Tworzy także węzeł wirusa macierzystego o identyfikatorze stem_id.
    explicit VirusGenealogy(typename Virus::id_type const &stem_id) {
        this->stem_id = stem_id;
        virus_map[stem_id] = std::make_shared<Node>(stem_id);
    }

    VirusGenealogy(VirusGenealogy<Virus> const &other) = delete;

    VirusGenealogy(VirusGenealogy<Virus> &&other) = delete;

    VirusGenealogy &operator=(VirusGenealogy<Virus> const &other) = delete;

    VirusGenealogy &operator=(VirusGenealogy<Virus> &&other) = delete;

    // Zwraca identyfikator wirusa macierzystego.
    typename Virus::id_type get_stem_id() const {
        return stem_id;
    }

    // Zwraca iterator pozwalający przeglądać listę identyfikatorów
    // bezpośrednich następników wirusa o podanym identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
    // Iterator musi spełniać koncept bidirectional_iterator oraz
    // typeid(*v.get_children_begin()) == typeid(const Virus &).
    VirusGenealogy<Virus>::children_iterator get_children_begin(typename Virus::id_type const &id) const {
        auto virus_node = check_if_virus_present(id)->second;
        return virus_node->get_children_begin();
    }

    // Iterator wskazujący na element za końcem wyżej wspomnianej listy.
    // Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
    VirusGenealogy<Virus>::children_iterator get_children_end(typename Virus::id_type const &id) const {
        auto virus_node = check_if_virus_present(id)->second;
        return virus_node->get_children_end();
    }

    // Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
    // o podanym identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
    std::vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const {
        auto virus_node = check_if_virus_present(id)->second;
        return virus_node->get_parents();
    }

    // Sprawdza, czy wirus o podanym identyfikatorze istnieje.
    bool exists(typename Virus::id_type const &id) const {
        return virus_map.find(id) != virus_map.end();
    }

    // Zwraca referencję do obiektu reprezentującego wirus o podanym
    // identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
    const Virus &operator[](typename Virus::id_type const &id) const {
        auto virus_node = check_if_virus_present(id)->second;
        return *(virus_node->get_virus());
    }

    // Tworzy węzeł reprezentujący nowy wirus o identyfikatorze id
    // powstały z wirusów o podanym identyfikatorze parent_id lub
    // podanych identyfikatorach parent_ids.
    // Zgłasza wyjątek VirusAlreadyCreated, jeśli wirus o identyfikatorze
    // id już istnieje.
    // Zgłasza wyjątek VirusNotFound, jeśli któryś z wyspecyfikowanych
    // poprzedników nie istnieje.
    void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
        create(id, std::vector<typename Virus::id_type>({ parent_id }));
    }

    void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids) {
        if (exists(id)) {
            throw VirusAlreadyCreated();
        }

        if (parent_ids.empty()) {
            return;
        }
        auto child_ptr = std::make_shared<Node>(id);
        std::vector<std::shared_ptr<Node>> parents;

        for (auto& parent: parent_ids) {
            auto parent_node = check_if_virus_present(parent)->second;
            parents.push_back(parent_node);
            child_ptr->add_parent(parent, parent_node);
        }

        auto new_node_ptr = virus_map.insert({ id, child_ptr });
        size_t addedCount = 0;
        try {
            for (auto& parent: parents) {
                parent->add_child(child_ptr->get_virus(), child_ptr);
                addedCount++;
            }
        }
        catch (...) {
            for (size_t i = 0; i < addedCount; ++i) {
                parents[i]->remove_child(child_ptr->get_virus());
            }
            virus_map.erase(new_node_ptr.first);
            throw;
        }
    }

    // Dodaje nową krawędź w grafie genealogii.
    // Zgłasza wyjątek VirusNotFound, jeśli któryś z podanych wirusów nie istnieje.
    void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id) {
        auto child = check_if_virus_present(child_id)->second;
        auto parent = check_if_virus_present(parent_id)->second;

        child->add_parent(parent_id, virus_map.at(parent_id));
        parent->add_child(child->get_virus(), virus_map.at(child_id));
    }

    // Usuwa wirus o podanym identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
    // Zgłasza wyjątek TriedToRemoveStemVirus przy próbie usunięcia
    // wirusa macierzystego.
    void remove(typename Virus::id_type const &id) {
        auto node_iterator = check_if_virus_present(id);
        if (id == get_stem_id()) {
            throw TriedToRemoveStemVirus();
        }
        auto node = node_iterator->second;
        std::vector<typename Virus::id_type> deleted = node->destroy();
        std::vector<typename virus_map_t::iterator> iterators;
        virus_map.erase(node_iterator);
        try {
            for (auto deletedId: deleted) {
                auto deleted_node_it = virus_map.find(deletedId);
                virus_map.erase(deleted_node_it);
                iterators.push_back(deleted_node_it);
            }
        }
        catch (...) {
            for (auto& deleted_it: iterators) {
                virus_map.insert(*deleted_it);
            }
            node->revert_destroy();
            throw;
        }
        node->commit_destroy(true);
    }
};

#endif //VIRUS_GENEALOGY_H
