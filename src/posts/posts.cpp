#include "posts.hpp"
#include "json.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>

using json = nlohmann::json;

namespace {
    const std::filesystem::path postsFilePath = "data/posts.json";
}

std::vector<Post> loadPosts()
{
    std::ifstream file(postsFilePath);

    if (!file.is_open()) {
        return {};
    }

    try {
        json data;
        file >> data;

        std::vector<Post> posts;

        for (const auto& item : data) {
            Post post;
            post.slug = item.at("slug").get<std::string>();
            post.title = item.at("title").get<std::string>();
            post.markdown = item.at("markdown").get<std::string>();
            post.createdAt = item.at("createdAt").get<std::string>();
            post.updatedAt = item.value("updatedAt", post.createdAt);

            posts.push_back(std::move(post));
        }

        return posts;
    } catch (const json::exception& error) {
        std::cerr << "Failed to load posts: " << error.what() << '\n';
        return {};
    }
}

bool savePosts(const std::vector<Post>& posts)
{
    std::filesystem::create_directories(postsFilePath.parent_path());

    json data = json::array();

    for (const Post& post : posts)
    {
        data.push_back({
            {"slug", post.slug},
            {"title", post.title},
            {"markdown", post.markdown},
            {"createdAt", post.createdAt},
            {"updatedAt", post.updatedAt}
        });
    }

    std::ofstream file(postsFilePath);

    if (!file.is_open())
    {
        return false;
    }

    file << data.dump(4);

    return file.good();
}