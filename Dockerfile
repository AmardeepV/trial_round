FROM python:3.12-slim

WORKDIR /app
copy requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY python_automation/ ./python_automation/

WORKDIR /app/python_automation

CMD ["python", "-m", "pytest", "-v"]
