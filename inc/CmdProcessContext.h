#pragma once

#include "Bulk.h"
#include "CmdInterpreter.h"
#include "IObservable.h"
#include "Metrics.h"

#include <functional>
#include <list>
#include <memory>

namespace bulk {

/**
 * @brief Класс котнекста обработчика команд.
 */
class CmdProcessContext : public IObservable  {
  public:
    /**
     * @brief Конструктор.
     * @param bulk_size - макисмальный размер блока команд.
     */
    explicit CmdProcessContext(size_t bulk_size) : interpreter_{bulk_size}, metrics_{"main"} {
    }

    ~CmdProcessContext() override = default;

    /**
     * @brief Подписка на получение блоков команд на вывод.
     * @param observer - подписчик.
     */
    void subscribe(const std::shared_ptr<IStreamWriter>& observer) final;

    /**
     * @brief Отписка от получения блоков команд на вывод.
     * @param observer - подписчик.
     */
    void unsubscribe(const std::shared_ptr<IStreamWriter>& observer) final;

    /**
     * @brief Обработка входной команды.
     * @param data - входной данные, содержащие команды.
     * @param size - размер входных данных.
     */
    void process(const char* data, std::size_t size);

    /**
     * @brief Вывод метрик в поток.
     * @param os - выходной поток.
     */
    void print_metrics(std::ostream& os);

  private:
    /// Тип обозревателя.
    using observer_t = std::weak_ptr<IStreamWriter>;

    /**
     * @brief Публикация блока команд.
     * @param bulk - блок команд.
     */
    void publish(const Bulk& bulk) final;


    /// Список обозревателей, ожидающих вывод содержимого пула.
    std::list<observer_t> observers_{};
    /// Блок команд.
    Bulk bulk_{};
    /// Интерпретатор команд.
    CmdInterpreter interpreter_;
    /// Метрики основного потока.
    Metrics metrics_;
    /// Строка, содержащая данные со входными комнады.
    std::string data_;
};

} // namespace bulk.
