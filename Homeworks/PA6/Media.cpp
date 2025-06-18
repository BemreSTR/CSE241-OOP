#include <iostream>    // For input/output operations (e.g., std::cout)
#include <vector>      // For dynamic arrays (e.g., std::vector)
#include <string>      // For string manipulation (e.g., std::string)
#include <algorithm>   // For algorithms like std::remove
#include <memory>      // For smart pointers (though not extensively used in the final version, good practice to include for modern C++)

// Forward declarations
// These declarations inform the compiler about the existence of these classes
// before their full definitions are encountered, resolving potential circular dependencies.
class DataObject;
class Playable;
class NonPlayable;
class Observer;
class Dataset;

// --- Interfaces for Visual and Non-Visual ---
// IVisual defines an interface for objects that have a visual dimension.
class IVisual {
public:
    virtual ~IVisual() = default; // Virtual destructor to ensure proper cleanup of derived classes
    virtual const std::string& get_dimension() const = 0; // Pure virtual function for getting dimensions
};

// INonVisual defines an interface for objects that do not have a visual dimension.
class INonVisual {
public:
    virtual ~INonVisual() = default; // Virtual destructor
};

// --- Abstract Interfaces for Behavior ---
// Playable defines an interface for objects that can be "played".
class Playable {
public:
    virtual ~Playable() = default;           // Virtual destructor
    virtual void info() const = 0;           // Pure virtual function to display information
    virtual const std::string& get_duration() const = 0; // Pure virtual function for getting duration
    virtual DataObject* get_data_object() = 0; // Pure virtual function to get the underlying DataObject
};

// NonPlayable defines an interface for objects that can be "viewed" but not played.
class NonPlayable {
public:
    virtual ~NonPlayable() = default;        // Virtual destructor
    virtual void info() const = 0;           // Pure virtual function to display information
    virtual DataObject* get_data_object() = 0; // Pure virtual function to get the underlying DataObject
};

// --- Base Media Object ---
// DataObject is the base class for all media objects, holding common properties.
class DataObject {
protected:
    std::string name;       // Name of the media object
    std::string other_info; // Additional information about the object

public:
    // Constructor initializes name and other_info. `std::move` is used for efficient string transfer.
    DataObject(std::string n, std::string o) : name(std::move(n)), other_info(std::move(o)) {}
    virtual ~DataObject() = default; // Virtual destructor for proper polymorphic deletion
    virtual void info() const = 0;   // Pure virtual function to display object-specific info
    const std::string& get_name() const { return name; } // Getter for the name
};

// --- Concrete Media Classes ---
// TextObject represents a text-based media object. It is NonPlayable and INonVisual.
class TextObject : public DataObject, public NonPlayable, public INonVisual {
public:
    // Constructor calls the base DataObject constructor.
    TextObject(std::string n, std::string o) : DataObject(std::move(n), std::move(o)) {}
    // Overrides info() to display text-specific information.
    void info() const override {
        std::cout << "[Text] " << name << " - Info: " << other_info << std::endl;
    }
    // Returns a pointer to this DataObject.
    DataObject* get_data_object() override { return this; }
};

// ImageObject represents an image media object. It is NonPlayable and IVisual.
class ImageObject : public DataObject, public NonPlayable, public IVisual {
    std::string dimension; // Stores the dimensions of the image
public:
    // Constructor initializes DataObject and dimension.
    ImageObject(std::string n, std::string d, std::string o)
        : DataObject(std::move(n), std::move(o)), dimension(std::move(d)) {}
    // Overrides info() to display image-specific information including dimensions.
    void info() const override {
        std::cout << "[Image] " << name << " - Dim: " << dimension << ", Info: " << other_info << std::endl;
    }
    // Overrides get_dimension() from IVisual.
    const std::string& get_dimension() const override { return dimension; }
    // Returns a pointer to this DataObject.
    DataObject* get_data_object() override { return this; }
};

// AudioObject represents an audio media object. It is Playable and INonVisual.
class AudioObject : public DataObject, public Playable, public INonVisual {
    std::string duration; // Stores the duration of the audio
public:
    // Constructor initializes DataObject and duration.
    AudioObject(std::string n, std::string d, std::string o)
        : DataObject(std::move(n), std::move(o)), duration(std::move(d)) {}
    // Overrides info() to display audio-specific information including duration.
    void info() const override {
        std::cout << "[Audio] " << name << " - Duration: " << duration << ", Info: " << other_info << std::endl;
    }
    // Overrides get_duration() from Playable.
    const std::string& get_duration() const override { return duration; }
    // Returns a pointer to this DataObject.
    DataObject* get_data_object() override { return this; }
};

// VideoObject represents a video media object. It is Playable and IVisual.
class VideoObject : public DataObject, public Playable, public IVisual {
    std::string duration;  // Stores the duration of the video
    std::string dimension; // Stores the dimensions of the video
public:
    // Constructor initializes DataObject, duration, and dimension.
    VideoObject(std::string n, std::string d, std::string dim, std::string o)
        : DataObject(std::move(n), std::move(o)), duration(std::move(d)), dimension(std::move(dim)) {}
    // Overrides info() to display video-specific information including duration and dimensions.
    void info() const override {
        std::cout << "[Video] " << name << " - Duration: " << duration << ", Dim: " << dimension << ", Info: " << other_info << std::endl;
    }
    // Overrides get_duration() from Playable.
    const std::string& get_duration() const override { return duration; }
    // Overrides get_dimension() from IVisual.
    const std::string& get_dimension() const override { return dimension; }
    // Returns a pointer to this DataObject.
    DataObject* get_data_object() override { return this; }
};

// --- Observer Interface ---
// Observer defines the interface for objects that want to be notified of changes in the Dataset.
class Observer {
public:
    virtual ~Observer() = default;               // Virtual destructor
    virtual void update_add(DataObject* obj) = 0;    // Pure virtual function to notify about an added object
    virtual void update_remove(DataObject* obj) = 0; // Pure virtual function to notify about a removed object
    virtual bool wants_playable() const = 0;     // Pure virtual function to indicate if observer wants playable objects
    virtual bool wants_non_playable() const = 0; // Pure virtual function to indicate if observer wants non-playable objects
};

// --- Dummy Objects ---
// DummyPlayable is a placeholder for when no playable item is available.
class DummyPlayable : public Playable {
public:
    void info() const override { std::cout << "[None] No item playing." << std::endl; }
    const std::string& get_duration() const override {
        static std::string dummy = "0"; // A static dummy duration
        return dummy;
    }
    DataObject* get_data_object() override { return nullptr; } // Returns nullptr as it's not a real DataObject
};

// DummyNonPlayable is a placeholder for when no non-playable item is available.
class DummyNonPlayable : public NonPlayable {
public:
    void info() const override { std::cout << "[None] No item viewing." << std::endl; }
    DataObject* get_data_object() override { return nullptr; } // Returns nullptr as it's not a real DataObject
};

// --- Player ---
// Player acts as an observer for Playable objects and manages a playlist.
class Player : public Observer {
    std::vector<Playable*> playlist; // List of playable objects
    int current = -1;                // Index of the currently playing item (-1 if none)
    DummyPlayable dummy;             // Dummy object for when playlist is empty

public:
    // Indicates that this observer wants playable objects.
    bool wants_playable() const override { return true; }
    // Indicates that this observer does not want non-playable objects.
    bool wants_non_playable() const override { return false; }

    // Called when a new object is added to the dataset.
    // If the object is Playable, it's added to the playlist.
    void update_add(DataObject* obj) override {
        auto* playable = dynamic_cast<Playable*>(obj);
        if (playable) {
            playlist.push_back(playable);
            if (current == -1) current = 0; // Set current to 0 if playlist was empty
        }
    }

    // Called when an object is removed from the dataset.
    // Removes the corresponding playable object from the playlist and adjusts current index.
    void update_remove(DataObject* obj) override {
        for (size_t i = 0; i < playlist.size(); ++i) {
            if (playlist[i]->get_data_object() == obj) {
                playlist.erase(playlist.begin() + i);
                // Adjust current index if the removed item was before or at the current position.
                // If the playlist becomes empty, set current to -1.
                if (current >= (int)playlist.size()) current = playlist.empty() ? -1 : 0;
                break;
            }
        }
    }

    // Returns the currently playing item, or the dummy object if none.
    Playable* currently_playing() {
        return (current != -1) ? playlist[current] : &dummy;
    }

    // Displays the current playlist.
    void show_list() const {
        std::cout << "--- Player Playlist ---" << std::endl;
        if (playlist.empty()) { std::cout << "(empty)\n"; return; }
        for (size_t i = 0; i < playlist.size(); ++i) {
            playlist[i]->info();
        }
    }

    // Navigates to the next item of a specific type in the playlist.
    // Cycles through the playlist until the next matching type is found or a full cycle is completed.
    void next(const std::string& type) {
        if (playlist.empty()) return;
        int size = playlist.size();
        int start = current; // Keep track of the starting point to detect full cycle
        do {
            current = (current + 1) % size; // Move to the next item, wrapping around
            auto* obj = playlist[current]->get_data_object();
            // Check if the current object matches the requested type
            if ((type == "audio" && dynamic_cast<AudioObject*>(obj)) ||
                (type == "video" && dynamic_cast<VideoObject*>(obj))) return;
        } while (current != start); // Continue until a full cycle is made without finding a match
    }

    // Navigates to the previous item of a specific type in the playlist.
    // Cycles through the playlist until the previous matching type is found or a full cycle is completed.
    void previous(const std::string& type) {
        if (playlist.empty()) return;
        int size = playlist.size();
        int start = current; // Keep track of the starting point to detect full cycle
        do {
            current = (current - 1 + size) % size; // Move to the previous item, wrapping around (handles negative results of -1)
            auto* obj = playlist[current]->get_data_object();
            // Check if the current object matches the requested type
            if ((type == "audio" && dynamic_cast<AudioObject*>(obj)) ||
                (type == "video" && dynamic_cast<VideoObject*>(obj))) return;
        } while (current != start); // Continue until a full cycle is made without finding a match
    }
};

// --- Viewer ---
// Viewer acts as an observer for NonPlayable objects and manages a viewlist.
class Viewer : public Observer {
    std::vector<NonPlayable*> viewlist; // List of non-playable objects
    int current = -1;                   // Index of the currently viewed item (-1 if none)
    DummyNonPlayable dummy;             // Dummy object for when viewlist is empty

public:
    // Indicates that this observer does not want playable objects.
    bool wants_playable() const override { return false; }
    // Indicates that this observer wants non-playable objects.
    bool wants_non_playable() const override { return true; }

    // Called when a new object is added to the dataset.
    // If the object is NonPlayable, it's added to the viewlist.
    void update_add(DataObject* obj) override {
        auto* np = dynamic_cast<NonPlayable*>(obj);
        if (np) {
            viewlist.push_back(np);
            if (current == -1) current = 0; // Set current to 0 if viewlist was empty
        }
    }

    // Called when an object is removed from the dataset.
    // Removes the corresponding non-playable object from the viewlist and adjusts current index.
    void update_remove(DataObject* obj) override {
        for (size_t i = 0; i < viewlist.size(); ++i) {
            if (viewlist[i]->get_data_object() == obj) {
                viewlist.erase(viewlist.begin() + i);
                // Adjust current index if the removed item was before or at the current position.
                // If the viewlist becomes empty, set current to -1.
                if (current >= (int)viewlist.size()) current = viewlist.empty() ? -1 : 0;
                break;
            }
        }
    }

    // Returns the currently viewed item, or the dummy object if none.
    NonPlayable* currently_viewing() {
        return (current != -1) ? viewlist[current] : &dummy;
    }

    // Displays the current viewlist.
    void show_list() const {
        std::cout << "--- Viewer Viewlist ---" << std::endl;
        if (viewlist.empty()) { std::cout << "(empty)\n"; return; }
        for (size_t i = 0; i < viewlist.size(); ++i) {
            viewlist[i]->info();
        }
    }

    // Navigates to the next item of a specific type in the viewlist.
    // Cycles through the viewlist until the next matching type is found or a full cycle is completed.
    void next(const std::string& type) {
        if (viewlist.empty()) return;
        int size = viewlist.size();
        int start = current; // Keep track of the starting point to detect full cycle
        do {
            current = (current + 1) % size; // Move to the next item, wrapping around
            auto* obj = viewlist[current]->get_data_object();
            // Check if the current object matches the requested type
            if ((type == "text" && dynamic_cast<TextObject*>(obj)) ||
                (type == "image" && dynamic_cast<ImageObject*>(obj))) return;
        } while (current != start); // Continue until a full cycle is made without finding a match
    }

    // Navigates to the previous item of a specific type in the viewlist.
    // Cycles through the viewlist until the previous matching type is found or a full cycle is completed.
    void previous(const std::string& type) {
        if (viewlist.empty()) return;
        int size = viewlist.size();
        int start = current; // Keep track of the starting point to detect full cycle
        do {
            current = (current - 1 + size) % size; // Move to the previous item, wrapping around
            auto* obj = viewlist[current]->get_data_object();
            // Check if the current object matches the requested type
            if ((type == "text" && dynamic_cast<TextObject*>(obj)) ||
                (type == "image" && dynamic_cast<ImageObject*>(obj))) return;
        } while (current != start); // Continue until a full cycle is made without finding a match
    }
};

// --- Dataset Class ---
// Dataset acts as the Subject in the Observer pattern, managing a collection of DataObjects
// and notifying registered Observers about additions and removals.
class Dataset {
    std::vector<DataObject*> objects;    // Stores all DataObjects in the dataset
    std::vector<Observer*> observers;    // Stores all registered Observers

public:
    // Registers an observer with the dataset.
    // When an observer registers, it immediately receives all existing relevant objects.
    void register_observer(Observer* obs) {
        observers.push_back(obs);
        // Notify the newly registered observer about existing objects
        for (auto* obj : objects) {
            if (obs->wants_playable() && dynamic_cast<Playable*>(obj)) obs->update_add(obj);
            if (obs->wants_non_playable() && dynamic_cast<NonPlayable*>(obj)) obs->update_add(obj);
        }
    }

    // Removes an observer from the dataset.
    void remove_observer(Observer* obs) {
        // Use std::remove-erase idiom to remove the observer
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    // Adds a new DataObject to the dataset.
    // Notifies all relevant registered observers about the addition.
    void add(DataObject* obj) {
        objects.push_back(obj);
        // Notify all observers
        for (auto* obs : observers) {
            if (obs->wants_playable() && dynamic_cast<Playable*>(obj)) obs->update_add(obj);
            if (obs->wants_non_playable() && dynamic_cast<NonPlayable*>(obj)) obs->update_add(obj);
        }
    }

    // Removes a DataObject from the dataset.
    // Notifies all relevant registered observers about the removal and then deletes the object.
    void remove(DataObject* obj) {
        // Notify all observers before removing the object from the dataset
        for (auto* obs : observers) {
            if (obs->wants_playable() && dynamic_cast<Playable*>(obj)) obs->update_remove(obj);
            if (obs->wants_non_playable() && dynamic_cast<NonPlayable*>(obj)) obs->update_remove(obj);
        }
        // Remove the object from the dataset's internal list
        objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
        delete obj; // Deallocate the memory for the DataObject
    }

    // Destructor of Dataset.
    // Ensures all dynamically allocated DataObjects are deleted to prevent memory leaks.
    ~Dataset() {
        for (auto* obj : objects) delete obj; // Delete all remaining DataObjects
    }
};

int main() {
    // Create a new Dataset instance
    Dataset* ds = new Dataset();

    // Observers are created
    Player* player1 = new Player();
    Player* player2 = new Player();
    Viewer* viewer1 = new Viewer();
    Viewer* viewer2 = new Viewer();

    // Observers are registered with the Dataset
    ds->register_observer(player1);
    ds->register_observer(player2);
    ds->register_observer(viewer1);
    ds->register_observer(viewer2);

    // Media objects are added
    ds->add(new ImageObject("image1", "1024x768", "wallpaper"));
    ds->add(new ImageObject("image2", "800x600", "icon"));
    ds->add(new AudioObject("audio1", "3:45", "song"));
    ds->add(new AudioObject("audio2", "1:15", "alarm"));
    ds->add(new VideoObject("video1", "10:00", "1920x1080", "documentary"));
    ds->add(new VideoObject("video2", "2:00", "640x360", "ad"));
    ds->add(new TextObject("text1", "poem"));
    ds->add(new TextObject("text2", "news"));

    std::cout << "\n== Initial lists ==\n";
    player1->show_list();
    viewer1->show_list();

    // First played and viewed objects
    std::cout << "\nPlayer1 currently playing:\n";
    player1->currently_playing()->info();
    std::cout << "\nViewer1 currently viewing:\n";
    viewer1->currently_viewing()->info();

    // Player1 navigates to the next audio
    std::cout << "\nPlayer1 next() for audio:\n";
    player1->next("audio");
    player1->currently_playing()->info();

    // Player1 navigates to the next video
    std::cout << "\nPlayer1 next() for video:\n";
    player1->next("video");
    player1->currently_playing()->info();

    // Viewer1 navigates to the next image
    std::cout << "\nViewer1 next() for image:\n";
    viewer1->next("image");
    viewer1->currently_viewing()->info();

    // Viewer1 navigates to the next text
    std::cout << "\nViewer1 next() for text:\n";
    viewer1->next("text");
    viewer1->currently_viewing()->info();

    // Previous tests when at the end of the list (or cycling)
    std::cout << "\nPlayer1 previous() for audio:\n";
    player1->previous("audio");
    player1->currently_playing()->info();

    std::cout << "\nViewer1 previous() for image:\n";
    viewer1->previous("image");
    viewer1->currently_viewing()->info();

    // Deleting the currently playing/viewing object and checking automatic transition to the next
    std::cout << "\nCurrently playing/viewing object is being deleted:\n";
    DataObject* removed_playing = player1->currently_playing()->get_data_object();
    DataObject* removed_viewing = viewer1->currently_viewing()->get_data_object();
    ds->remove(removed_playing); // Will be removed from dataset and players
    ds->remove(removed_viewing); // Will be removed from dataset and viewers

    std::cout << "\nPlayer1 playlist updated:\n";
    player1->show_list();
    std::cout << "Currently playing: ";
    player1->currently_playing()->info();

    std::cout << "\nViewer1 viewlist updated:\n";
    viewer1->show_list();
    std::cout << "Currently viewing: ";
    viewer1->currently_viewing()->info();

    // Unregistering an observer to check for non-update upon new object addition
    std::cout << "\nPlayer2 observer is being removed, it won't be updated when a new object is added:\n";
    ds->remove_observer(player2);
    ds->add(new AudioObject("audio3", "2:00", "podcast"));
    player2->show_list(); // Should show that the new object was not added

    // Testing the scenario where all objects are deleted, falling back to dummy objects
    std::cout << "\nAll objects are being deleted...\n";
    while (true) {
        Playable* cur = player1->currently_playing();
        DataObject* dobj = cur->get_data_object();
        if (dobj)
            ds->remove(dobj);
        else
            break; // No more real DataObjects, dummy is active
    }
    player1->show_list();
    std::cout << "Currently playing: ";
    player1->currently_playing()->info();

    while (true) {
        NonPlayable* cur = viewer1->currently_viewing();
        DataObject* dobj = cur->get_data_object();
        if (dobj)
            ds->remove(dobj);
        else
            break; // No more real DataObjects, dummy is active
    }
    viewer1->show_list();
    std::cout << "Currently viewing: ";
    viewer1->currently_viewing()->info();

    // Test for memory leaks when the Dataset is deleted (for use with Valgrind or similar tools)
    delete ds;
    delete player1;
    delete player2;
    delete viewer1;
    delete viewer2;

    return 0;
}