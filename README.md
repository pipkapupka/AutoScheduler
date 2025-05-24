# 🎓 AutoScheduler — система автоматизации учебных расписаний
[![Progress](https://img.shields.io/badge/Progress-5%25-red)](https://github.com/yourusername/AutoScheduler)
[![C++](https://img.shields.io/badge/C++-20-blue)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C)](https://cmake.org)
[![Docker](https://img.shields.io/badge/Docker-Configured-success)](https://www.docker.com)
[![Last Commit](https://img.shields.io/github/last-commit/pipkapupka/AutoScheduler)](https://github.com/yourusername/AutoScheduler/commits/main)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)
---
**О проекте**: 

> C++ проект для генерации и оптимизации расписаний с использованием алгоритмов и микросервисной архитектуры. 
> Цель: сократить время составления расписаний и адаптироваться к изменениям в реальном времени.

## 🛠 Технологический стек
| Компонент       | Технологии                          |
|-----------------|-------------------------------------|
| **Ядро**        | C++20 (STL, многопоточность)        |
| **Алгоритмы**   | Жадные алгоритмы → [Планируется: нейросеть] |
| **База данных** | PostgreSQL                          |
| **API**         | gRPC, RestAPI                       |
| **Инфраструктура** | Docker, Kubernetes (в планах)    |

## 📦 Текущий функционал
✅ **Реализовано**:
- Несколько микросервисов для работы с API колледжа, который в будущем будет переписываться под шаблонный сервис

🛠 **В разработке**:
- CLI клиент для тестирования запросов к сервисам
- Оптимизация архитектуры на подходящие STL-контейнеры
- Связка существующих микросервисов для общения между собой

✍🏻 **В планах**
- Начать писать ядро на жадных алгоритмах для тестирования на синтетических входных данных
- Разделение на gitflow ветки для дробления 
