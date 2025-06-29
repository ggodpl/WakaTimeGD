#pragma once
#include <string>
#include <chrono>

namespace tracker {
    enum class ActivityCategory {
        Advising,
        Browsing,
        Building,
        CodeReviewing,
        Coding,
        Communicating,
        Debugging,
        Designing,
        Indexing,
        Learning,
        ManualTesting,
        Meeting,
        Planning,
        Researching,
        RunningTests,
        Supporting,
        Translating,
        WritingDocs,
        WritingTests,
        None,
    };

    class ActivityTracker {
        private:
            bool paused = false;
            std::string currentProject;
            ActivityCategory currentCategory = ActivityCategory::None;
            bool active = false;
            bool stopping = false;

            std::chrono::system_clock::time_point activityStarted;

            ActivityTracker() = default;

            void onActivityChange();
            void checkActivity();
            void logActivity(const std::string& project, ActivityCategory category, int duration);
            void update(const std::string& project, ActivityCategory cateory, int duration, bool allTime);
        public:
            static ActivityTracker& getInstance() {
                static ActivityTracker instance;
                return instance;
            };

            void pause();
            void resume();
            bool canHB() const;

            void resetActivity();
            void updateActivity(const std::string& project, ActivityCategory category);

            std::string getProject() const;
            std::string getCategory() const;
    };
}