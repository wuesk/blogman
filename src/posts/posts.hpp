#pragma once

#include <string>
#include <vector>

struct Post
{
    std::string slug;
    std::string title;
    std::string markdown;
    std::string createdAt;
    std::string updatedAt;
};

std::vector<Post> loadPosts();
bool savePosts(const std::vector<Post>& posts);