FROM php:8.1-apache

# Установка необходимых расширений PHP, если нужны (например, для MySQL)
RUN docker-php-ext-install mysqli pdo pdo_mysql

# Копирование файлов сайта в контейнер
COPY . /var/www/html/

# Установка прав доступа
RUN chown -R www-data:www-data /var/www/html
RUN chmod -R 755 /var/www/html

# Включение модуля rewrite, если он нужен
RUN a2enmod rewrite

EXPOSE 80
