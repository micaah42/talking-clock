import pandas as pd

from typing import Optional
from typing import List
from sqlalchemy import ForeignKey
from sqlalchemy import String
from sqlalchemy.orm import DeclarativeBase
from sqlalchemy.orm import mapped_column
from sqlalchemy.orm import Session
from sqlalchemy.orm import Mapped
from sqlalchemy import create_engine

class Base(DeclarativeBase):
    pass

class ActionDay(Base):
    __tablename__ = "action_day"
    id: Mapped[int] = mapped_column(primary_key=True)
    date: Mapped[str] = mapped_column(String(5))
    name: Mapped[str] = mapped_column(String(32))
    desc: Mapped[str] = mapped_column(String(32))
    link: Mapped[str] = mapped_column(String(32))
    icon: Mapped[str] = mapped_column(String(32))
    
engine = create_engine("sqlite:///sources/actiondays.sqlite", echo=True)
Base.metadata.create_all(engine)

actiondays = pd.read_csv('./sources/actiondays-v2.csv')
actiondays = [ActionDay(**(action_day.to_dict())) for _, action_day in actiondays.iterrows()]

with Session(engine) as session:
    session.add_all(actiondays)
    session.commit()